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

/*! \file nas_mme_task.c
   \brief
   \author  Sebastien ROUX, Lionel GAUTHIER
   \date
   \email: lionel.gauthier@eurecom.fr
*/
#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bstrlib.h"
#include "emm_main.h"
#include "nas_emm.h"
#include "nas_emm_proc.h"
#include "nas_timer.h"

#include "assertions.h"
#include "common_defs.h"
#include "intertask_interface.h"
#include "itti_free_defined_msg.h"
#include "log.h"
#include "mme_config.h"
#include "msc.h"
#include "nas_network.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

# define PORT 10400

static void nas_emm_exit(void);


int findNull(char *string, int size)
{
  int i;
  for(i=0;i<size;i++)
  {
    if(string[i] == '\0')
      break;
  }
  return i-1;
}

int append(char *message, char* string, int actualSize, int prev)
{
  int i;
  int start, end;
  if(prev == 0)
  {
    start = 0;
    end = actualSize;
  }
  else
  {
    start = prev + 2;
    end = start + actualSize;
  }

  int j=0;

  for(i=start; i <= end; i++)
  {
     message[i] = string[j];
     j++;
  }
  message[end+1] = ' ';
  return end;
}

void emptyArr(char* string, int size)
{
  int i;
  for(i=0; i<size;i++)
    string[i] = '\0';
}

void startClient(s6a_auth_info_ans_t *aia)
{
  char message[2048];
  int i;
  for(i=0; i<2048;i++)
    message[i] = '0';

  char string_res[255];
  emptyArr(string_res, 255);
  char string_rand[512];
  emptyArr(string_rand, 512);
  char string_kasme[512];
  emptyArr(string_kasme, 512);
  char string_autn[512];
  emptyArr(string_autn, 512);

  int size_xres = sizeof(aia->auth_info.eutran_vector[0].xres.data)/sizeof(__uint8_t);
  int size_rand = sizeof(aia->auth_info.eutran_vector[0].rand)/sizeof(__uint8_t);
  int size_kasme = sizeof(aia->auth_info.eutran_vector[0].kasme)/sizeof(__uint8_t);
  int size_autn = sizeof(aia->auth_info.eutran_vector[0].autn)/sizeof(__uint8_t);

  // convert 8 bit arrays to strings 
  
  for(size_t i = 0; i < size_xres; i++)
      sprintf( &string_res[ strlen(string_res) ],  "%02x", aia->auth_info.eutran_vector[0].xres.data[i] );
  //OAILOG_DEBUG(LOG_S6A, "XRES %s\n", string_res);

  for(size_t i = 0; i < size_rand; i++)
      sprintf( &string_rand[ strlen(string_rand) ],  "%02x", aia->auth_info.eutran_vector[0].rand[i] );
  //OAILOG_DEBUG(LOG_S6A, "RAND %s\n", string_rand);

  for(size_t i = 0; i < size_kasme; i++)
      sprintf( &string_kasme[ strlen(string_kasme) ],  "%02x",aia->auth_info.eutran_vector[0].kasme[i] );
  //OAILOG_DEBUG(LOG_S6A, "KASME %s\n", string_kasme);
 
  for(size_t i = 0; i < size_autn; i++)
    sprintf( &string_autn[ strlen(string_autn) ],  "%02x", aia->auth_info.eutran_vector[0].autn[i] );
  //OAILOG_DEBUG(LOG_S6A, "AUTN %s\n", string_autn);

  size_xres = aia->auth_info.eutran_vector[0].xres.size * 2;

  // append all vector parameters to a single string

  int end = append(message, string_res, size_xres-1, 0);
  int actualSize = findNull(string_rand, size_rand*2);
  end = append(message, string_rand, actualSize, end);
  actualSize = findNull(string_kasme, size_kasme*2);
  end = append(message, string_kasme, actualSize, end);
  actualSize = findNull(string_autn, size_autn*2);
  end = append(message, string_autn, actualSize, end);

  message[end+1] = '\0';
  //OAILOG_DEBUG(LOG_S6A, "Message %s\n", message);

  int sock;
	struct sockaddr_in server;
  sock = socket(AF_INET , SOCK_STREAM , 0);

	if (sock == -1)
	{
		OAILOG_DEBUG(LOG_S6A, "Could not create client socket\n");
    return;
	}
	OAILOG_DEBUG(LOG_S6A, "Client socket created\n");
	
	server.sin_addr.s_addr = inet_addr("192.168.0.100");
	server.sin_family = AF_INET;
	server.sin_port = htons( PORT );

	//Connect to remote server
	if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
	{
		OAILOG_DEBUG(LOG_S6A, "Connection to server failed. Error\n");
		return;
	}
	OAILOG_DEBUG(LOG_S6A, "Connected to server\n");
	

	//keep communicating with server

	if( send(sock , message , strlen(message) , 0) < 0)
	{
		OAILOG_DEBUG(LOG_S6A, "Send failed\n");
		return;
	}
		
	close(sock);
  return;
}

//------------------------------------------------------------------------------
static void *nas_emm_intertask_interface(void *args_p) {
  itti_mark_task_ready(TASK_NAS_EMM);

  while (1) {
    MessageDef *received_message_p = NULL;

    itti_receive_msg(TASK_NAS_EMM, &received_message_p);

    switch (ITTI_MSG_ID(received_message_p)) {
      case MESSAGE_TEST: {
        OAI_FPRINTF_INFO("TASK_NAS_EMM received MESSAGE_TEST\n");
      } break;

        /*
         * We don't need the S-TMSI: if with the given UE_ID we can find an EMM
         * context, that means, that a valid UE context could be matched for the
         * UE context, and we can continue with it.
         */
      case NAS_INITIAL_UE_MESSAGE: {
        nas_establish_ind_t *nas_est_ind_p = NULL;
        nas_est_ind_p = &received_message_p->ittiMsg.nas_initial_ue_message.nas;
        nas_proc_establish_ind(nas_est_ind_p->ue_id, nas_est_ind_p->tai,
                               nas_est_ind_p->ecgi, nas_est_ind_p->as_cause,
                               &nas_est_ind_p->initial_nas_msg);
      } break;

      case NAS_DL_DATA_CNF: {
        nas_proc_dl_transfer_cnf(NAS_DL_DATA_CNF(received_message_p).ue_id,
                                 NAS_DL_DATA_CNF(received_message_p).err_code,
                                 &NAS_DL_DATA_REJ(received_message_p).nas_msg);
      } break;

      case NAS_UPLINK_DATA_IND: {
        nas_proc_ul_transfer_ind(
            NAS_UPLINK_DATA_IND(received_message_p).ue_id,
            NAS_UPLINK_DATA_IND(received_message_p).tai,
            NAS_UPLINK_DATA_IND(received_message_p).cgi,
            &NAS_UPLINK_DATA_IND(received_message_p).nas_msg);
      } break;

      case NAS_DL_DATA_REJ: {
        nas_proc_dl_transfer_rej(NAS_DL_DATA_REJ(received_message_p).ue_id,
                                 NAS_DL_DATA_REJ(received_message_p).err_code,
                                 &NAS_DL_DATA_REJ(received_message_p).nas_msg);
      } break;

      case NAS_IMPLICIT_DETACH_UE_IND: {
        nas_proc_implicit_detach_ue_ind(
            NAS_IMPLICIT_DETACH_UE_IND(received_message_p).ue_id,
            NAS_IMPLICIT_DETACH_UE_IND(received_message_p).emm_cause,
            NAS_IMPLICIT_DETACH_UE_IND(received_message_p).detach_type,
            NAS_IMPLICIT_DETACH_UE_IND(received_message_p).clr);
      } break;

      case S1AP_DEREGISTER_UE_REQ: {
        nas_proc_deregister_ue(
            S1AP_DEREGISTER_UE_REQ(received_message_p).mme_ue_s1ap_id);
      } break;

      case S6A_AUTH_INFO_ANS: {
        startClient(&S6A_AUTH_INFO_ANS(received_message_p));
        /*
         * We received the authentication vectors from HSS, trigger a ULR
         * for now. Normaly should trigger an authentication procedure with UE.
         */
        nas_proc_authentication_info_answer(
            &S6A_AUTH_INFO_ANS(received_message_p));
      } break;

      case NAS_CONTEXT_RES: {
        nas_proc_context_res(&NAS_CONTEXT_RES(received_message_p));
      } break;

      case NAS_CONTEXT_FAIL: {
        nas_proc_context_fail(NAS_CONTEXT_FAIL(received_message_p).ue_id,
                              NAS_CONTEXT_FAIL(received_message_p).cause);
      } break;

      case NAS_SIGNALLING_CONNECTION_REL_IND: {
        nas_proc_signalling_connection_rel_ind(
            NAS_SIGNALLING_CONNECTION_REL_IND(received_message_p).ue_id);
      } break;

      case TERMINATE_MESSAGE: {
        nas_emm_exit();
        OAI_FPRINTF_INFO("TASK_NAS_EMM terminated\n");
        itti_free_msg_content(received_message_p);
        itti_free(ITTI_MSG_ORIGIN_ID(received_message_p), received_message_p);
        itti_exit_task();
      } break;

      case TIMER_HAS_EXPIRED: {
        /*
         * Call the NAS timer api
         */
        nas_timer_handle_signal_expiry(
            TIMER_HAS_EXPIRED(received_message_p).timer_id,
            TIMER_HAS_EXPIRED(received_message_p).arg);
        free_wrapper((void **)&TIMER_HAS_EXPIRED(received_message_p).arg);
      } break;

      default: {
        OAILOG_DEBUG(LOG_NAS, "Unknown message ID %d:%s from %s\n",
                     ITTI_MSG_ID(received_message_p),
                     ITTI_MSG_NAME(received_message_p),
                     ITTI_MSG_ORIGIN_NAME(received_message_p));
      } break;
    }

    itti_free_msg_content(received_message_p);
    itti_free(ITTI_MSG_ORIGIN_ID(received_message_p), received_message_p);
    received_message_p = NULL;
  }

  return NULL;
}

//------------------------------------------------------------------------------
int nas_emm_init(mme_config_t *mme_config_p) {
  OAILOG_DEBUG(LOG_NAS, "Initializing NAS EMM task interface\n");
  nas_timer_init();
  emm_main_initialize(mme_config_p);

  if (itti_create_task(TASK_NAS_EMM, &nas_emm_intertask_interface, NULL) < 0) {
    OAILOG_ERROR(LOG_NAS, "Create NAS EMM task failed");
    return -1;
  }

  OAILOG_DEBUG(LOG_NAS, "Initializing NAS EMM task interface: DONE\n");
  return 0;
}

//------------------------------------------------------------------------------
static void nas_emm_exit(void) {
  OAILOG_DEBUG(LOG_NAS, "Cleaning NAS EMM task interface\n");
  emm_main_cleanup();
  nas_timer_cleanup();
  OAILOG_DEBUG(LOG_NAS, "Cleaning NAS EMM task interface: DONE\n");
}
