/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ascordil <ascordil@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 01:17:07 by ascordil          #+#    #+#             */
/*   Updated: 2025/07/05 21:14:20 by ascordil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ratp.h"

int main(void) {

  CURLcode res;
  const char *code_porte_clichy = "71545";
  size_t content_size = 0;
  char *content = NULL;

  res = do_request(code_porte_clichy, &content, &content_size);
  if (res != CURLE_OK) {
    fprintf(stderr, "Error: %s\n", curl_easy_strerror(res));
    return (1);
  } else {
    print_all_next_passages(content);
    print_next_passage(content);
    print_last_passage(content);
    free(content);
  }
  return 0;
}
