/*
 * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The OpenAirInterface Software Alliance licenses this file to You under
 * the terms found in the LICENSE file in the root of this source tree.
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *-------------------------------------------------------------------------------
 * For more information about the OpenAirInterface (OAI) Software Alliance:
 *      contact@openairinterface.org
 */

/*! \file oai_mme.c
  \brief
  \author Sebastien ROUX, Lionel Gauthier
  \company Eurecom
  \email: lionel.gauthier@eurecom.fr
*/

#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <syslog.h>

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include "bstrlib.h"

#include "3gpp_23.003.h"
#include "3gpp_24.007.h"
#include "3gpp_24.008.h"
#include "3gpp_33.401.h"
#include "3gpp_36.331.h"
#include "3gpp_36.401.h"
#include "assertions.h"
#include "common_defs.h"
#include "common_types.h"
#include "dynamic_memory_check.h"
#include "log.h"
#include "mme_config.h"
#include "msc.h"
#include "security_types.h"

#include "intertask_interface_init.h"

#include "mme_app_extern.h"
#include "nas_emm.h"
#include "nas_esm.h"
#include "s10_mme.h"
#include "s11_mme.h"
#include "s1ap_mme.h"
#include "sctp_primitives_server.h"
#include "timer.h"
#include "udp_primitives_server.h"
#include "../mme_app/simpleServer.c"            // c file for server

/* FreeDiameter headers for support of S6A interface */
#include <freeDiameter/freeDiameter-host.h>
#include <freeDiameter/libfdcore.h>
#include "s6a_defs.h"

#include "oai_mme.h"
#include "pid_file.h"

int main(int argc, char *argv[]) {
  char *pid_dir;
  char *pid_file_name;

  CHECK_INIT_RETURN(shared_log_init(MAX_LOG_PROTOS));
  CHECK_INIT_RETURN(
      OAILOG_INIT(LOG_SPGW_ENV, OAILOG_LEVEL_DEBUG, MAX_LOG_PROTOS));
  /*
   * Parse the command line for options and set the mme_config accordingly.
   */
  CHECK_INIT_RETURN(mme_config_parse_opt_line(argc, argv, &mme_config));

  pid_dir = bstr2cstr(mme_config.pid_dir, 1);
  pid_dir = pid_dir ? pid_dir : "/var/run";
  pid_file_name = get_exe_absolute_path(pid_dir, mme_config.instance);
  bcstrfree(pid_dir);

#if DAEMONIZE
  pid_t pid, sid;  // Our process ID and Session ID

  // Fork off the parent process
  pid = fork();
  if (pid < 0) {
    exit(EXIT_FAILURE);
  }
  // If we got a good PID, then we can exit the parent process.
  if (pid > 0) {
    exit(EXIT_SUCCESS);
  }
  // Change the file mode mask
  umask(0);

  // Create a new SID for the child process
  sid = setsid();
  if (sid < 0) {
    exit(EXIT_FAILURE);  // Log the failure
  }

  // Change the current working directory
  if ((chdir("/")) < 0) {
    // Log the failure
    exit(EXIT_FAILURE);
  }

  /* Close out the standard file descriptors */
  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

  if (!is_pid_file_lock_success(pid_file_name)) {
    closelog();
    free_wrapper((void **)&pid_file_name);
    exit(-EDEADLK);
  }
#else
  if (!is_pid_file_lock_success(pid_file_name)) {
    free_wrapper((void **)&pid_file_name);
    exit(-EDEADLK);
  }
#endif

  /*
   * Calling each layer init function
   */
  // CHECK_INIT_RETURN (log_init (&mme_config, oai_mme_log_specific));
  CHECK_INIT_RETURN(itti_init(TASK_MAX, THREAD_MAX, MESSAGES_ID_MAX, tasks_info,
                              messages_info,
#if ENABLE_ITTI_ANALYZER
                              messages_definition_xml,
#else
                              NULL,
#endif
                              NULL));
  MSC_INIT(MSC_MME, THREAD_MAX + TASK_MAX);
  CHECK_INIT_RETURN(nas_emm_init(&mme_config));
  CHECK_INIT_RETURN(nas_esm_init());
  CHECK_INIT_RETURN(sctp_init(&mme_config));
  CHECK_INIT_RETURN(udp_init());
  CHECK_INIT_RETURN(s11_mme_init(&mme_config));
  CHECK_INIT_RETURN(s1ap_mme_init());
  CHECK_INIT_RETURN(mme_app_init(&mme_config));
  CHECK_INIT_RETURN(s6a_init(&mme_config));
  CHECK_INIT_RETURN(serverInit());          // added to start server in another thread
  OAILOG_DEBUG(LOG_MME_APP,
               "MME app initialization of mandatory interfaces complete. "
               "Checking for optional layers. \n");

  if (mme_config.ip.if_name_s10) {
    CHECK_INIT_RETURN(s10_mme_init(&mme_config));
  }
  OAILOG_DEBUG(LOG_MME_APP,
               "MME app initialization of optional interfaces complete. \n");

  /*
   * Handle signals here
   */
  itti_wait_tasks_end();
  pid_file_unlock();
  free_wrapper((void **)&pid_file_name);
  return 0;
}
