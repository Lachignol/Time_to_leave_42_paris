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

#include <cjson/cJSON.h>
#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <time.h>

#define APIKEY "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
#define BUFFER_SIZE 20000

struct MemoryStruct {
  char *memory;
  size_t size;
};

typedef struct {
  char direction[128];
  const char *time_str;
  time_t time_val;
} NextPassageInfo;

typedef struct {
  char direction[128];
  const char *time_str;
  time_t time_val;
} LastPassageInfo;

CURLcode do_request(const char *stop_point, char **content,
                    size_t *content_size);
void print_formatted_datetime(const char *iso);
void print_all_next_passages(const char *json);
void print_next_passage(const char *json);
void print_last_passage(const char *json);
char *trim(char *str);
void remove_accents(char *str);
void remove_accents_utf8(char *str);

#endif
