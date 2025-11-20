/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ascordil <ascordil@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 23:53:05 by ascordil          #+#    #+#             */
/*   Updated: 2025/05/21 01:24:43 by ascordil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ratp.h"

static size_t write_callback(void *contents, size_t size, size_t nmemb,
                             void *userp) {
  char *ptr;
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)userp;

  ptr = (char *)realloc(mem->memory, mem->size + realsize + 1);
  if (!ptr) {
    fprintf(stderr, "Erreur : mémoire insuffisante dans write_callback\n");
    return 0;
  }

  mem->memory = ptr;
  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = '\0';

  return realsize;
}

CURLcode do_request(const char *stop_point, char **content,
                    size_t *content_size) {
  CURL *curl;
  CURLcode res;
  struct MemoryStruct chunk;

  // Allocation initiale du buffer
  chunk.memory = (char *)malloc(BUFFER_SIZE);
  if (!chunk.memory)
    return CURLE_OUT_OF_MEMORY;
  chunk.size = 0;

  curl_global_init(CURL_GLOBAL_ALL);
  curl = curl_easy_init();

  if (!curl) {
    free(chunk.memory);
    curl_global_cleanup();
    return CURLE_FAILED_INIT;
  }

  char url[512];
  snprintf(url, sizeof(url),
           "https://prim.iledefrance-mobilites.fr/marketplace/"
           "stop-monitoring?MonitoringRef=STIF:StopPoint:Q:%s:",
           stop_point);

  struct curl_slist *headers = NULL;
  headers = curl_slist_append(headers, "Accept: application/json");
  headers = curl_slist_append(headers, "apikey: " APIKEY);

  // Configurer la requête
  curl_easy_setopt(curl, CURLOPT_URL, url);
  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

  // Exécuter la requête
  res = curl_easy_perform(curl);

  // Nettoyage
  curl_slist_free_all(headers);
  curl_easy_cleanup(curl);
  curl_global_cleanup();

  if (res == CURLE_OK) {
    *content = chunk.memory;
    *content_size = chunk.size;
    // DEBUG
    //  printf("content : %s\n", *content);
  } else {
    free(chunk.memory);
    *content = NULL;
    *content_size = 0;
  }

  return res;
}
