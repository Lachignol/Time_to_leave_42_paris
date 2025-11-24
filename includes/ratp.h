/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ratp.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ascordil <ascordil@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 23:54:22 by ascordil          #+#    #+#             */
/*   Updated: 2025/05/21 22:55:12 by ascordil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _RATP_H
#define _RATP_H

#define _DEFAULT_SOURCE
#define _XOPEN_SOURCE 700

#include "cJSON.h"
#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <time.h>

#define APIKEY "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
#define BUFFER_SIZE 20000

#define COLOR_YELLOW "\x1b[33m"
#define COLOR_RESET "\x1B[0m"
#define COLOR_GREEN "\x1B[1;32m" // Gras + Vert
#define COLOR_BLUE "\x1B[1;34m"  // gras+ bleu

struct MemoryStruct {
  char *memory;
  size_t size;
};

CURLcode do_request(const char *stop_point, char **content,
                    size_t *content_size);

void print_ascii_tag(void);
void print_last_passages_clichy_ligne_14(const char *json);
void print_next_passages_clichy_ligne_14(const char *json);
void print_formatted_datetime(const char *iso);
void print_formatted_time_only(const char *iso);
void print_formatted_time_only_colored(const char *iso, const char *color);
void print_all_next_passages_of_station(const char *json);
char *trim(char *str);
void remove_accents(char *str);
void remove_accents_utf8(char *str);

#endif
