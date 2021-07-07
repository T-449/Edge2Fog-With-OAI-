/*
 * Copyright (c) 2015, EURECOM (www.eurecom.fr)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation are
 * those of the authors and should not be interpreted as representing official
 * policies, either expressed or implied, of the FreeBSD Project.
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define G_LOG_DOMAIN ("PARSER")

#include "reference_type.h"
#include "ui_interface.h"

int reference_dissect_from_buffer(types_t* type,
                                  ui_set_signal_text_cb_t ui_set_signal_text_cb,
                                  gpointer user_data, buffer_t* buffer,
                                  uint32_t offset, uint32_t parent_offset,
                                  int indent, gboolean new_line) {
  DISPLAY_PARSE_INFO("reference", type->name, offset, parent_offset);

  if (type->name) {
    INDENTED(stdout, indent, fprintf(stdout, "<%s>\n", type->name));
  }

  if (type->child != NULL)
    type->child->type_dissect_from_buffer(
        type->child, ui_set_signal_text_cb, user_data, buffer, offset,
        parent_offset, type->name == NULL ? indent : indent + DISPLAY_TAB_SIZE,
        TRUE);

  if (type->name) {
    INDENTED(stdout, indent, fprintf(stdout, "</%s>\n", type->name));
  }

  return 0;
}

int reference_type_file_print(types_t* type, int indent, FILE* file) {
  if (type == NULL) return -1;

  INDENTED(file, indent, fprintf(file, "<Reference>\n"));
  INDENTED(file, indent + 4, fprintf(file, "Name .......: %s\n", type->name));
  INDENTED(file, indent + 4, fprintf(file, "Id .........: %d\n", type->id));
  INDENTED(file, indent + 4,
           fprintf(file, "Type .......: %d\n", type->type_xml));
  INDENTED(file, indent + 4, fprintf(file, "Size .......: %d\n", type->size));
  INDENTED(file, indent + 4, fprintf(file, "Align ......: %d\n", type->align));
  INDENTED(file, indent + 4,
           fprintf(file, "Context ....: %d\n", type->context));
  INDENTED(file, indent + 4, fprintf(file, "Offset .....: %d\n", type->offset));
  INDENTED(file, indent + 4, fprintf(file, "Line .......: %d\n", type->line));
  INDENTED(file, indent + 4, fprintf(file, "File .......: %s\n", type->file));

  if (type->child != NULL)
    type->child->type_file_print(type->child, indent + 4, file);

  if (type->file_ref != NULL)
    type->file_ref->type_file_print(type->file_ref, indent + 4, file);

  INDENTED(file, indent, fprintf(file, "</Reference>\n"));
  return 0;
}

int reference_type_hr_display(types_t* type, int indent) {
  if (type == NULL) return -1;

  INDENTED(stdout, indent, printf("<Reference>\n"));
  INDENTED(stdout, indent + 4, printf("Name .......: %s\n", type->name));
  INDENTED(stdout, indent + 4, printf("Id .........: %d\n", type->id));
  INDENTED(stdout, indent + 4, printf("Type .......: %d\n", type->type_xml));
  INDENTED(stdout, indent + 4, printf("Size .......: %d\n", type->size));
  INDENTED(stdout, indent + 4, printf("Align ......: %d\n", type->align));
  INDENTED(stdout, indent + 4, printf("Context ....: %d\n", type->context));
  INDENTED(stdout, indent + 4, printf("Offset .....: %d\n", type->offset));
  INDENTED(stdout, indent + 4, printf("Line .......: %d\n", type->line));
  INDENTED(stdout, indent + 4, printf("File .......: %s\n", type->file));

  if (type->child != NULL)
    type->child->type_hr_display(type->child, indent + 4);

  if (type->file_ref != NULL)
    type->file_ref->type_hr_display(type->file_ref, indent + 4);

  INDENTED(stdout, indent, printf("</Reference>\n"));
  return 0;
}
