#ifndef _GSC_UTILS_HPP_
#define _GSC_UTILS_HPP_

/* gsc functions */
#include "gsc.hpp"

void gsc_utils_sendcommandtoclient();

void gsc_utils_logprintconsole();

void gsc_utils_tolower();
void gsc_utils_replace();

void gsc_utils_file_exists();
void gsc_utils_fopen();
void gsc_utils_fread();
void gsc_utils_fwrite();
void gsc_utils_fclose();

void gsc_utils_getsystemtime();

void gsc_utils_getconfigstring();
void gsc_utils_makelocalizedstring();

#endif