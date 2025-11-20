/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ascordil <ascordil@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 00:47:19 by ascordil          #+#    #+#             */
/*   Updated: 2025/05/21 01:42:58 by ascordil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ratp.h"

char *trim(char *str) {
  char *end;

  while (*str == ' ' || *str == '\t' || *str == '\n' || *str == '\r')
    str++;
  if (*str == 0)
    return (str);
  end = str + strlen(str) - 1;
  while (end > str &&
         (*end == ' ' || *end == '\t' || *end == '\n' || *end == '\r'))
    end--;
  *(end + 1) = 0;
  return (str);
}

const char *accents_utf8[] = {"â", "a", "ô", "o", "ê",  "e", "î",
                              "i", "û", "u", "é", "e",  "è", "e",
                              "à", "a", "ç", "c", NULL, NULL};

void remove_accents_utf8(char *str) {
  char buffer[1024];
  int i = 0, j = 0;
  while (str[i]) {
    int replaced = 0;
    for (int k = 0; accents_utf8[k]; k += 2) {
      size_t len = strlen(accents_utf8[k]);
      if (strncmp(&str[i], accents_utf8[k], len) == 0) {
        size_t repl_len = strlen(accents_utf8[k + 1]);
        strncpy(&buffer[j], accents_utf8[k + 1], repl_len);
        j += repl_len;
        i += len;
        replaced = 1;
        break;
      }
    }
    if (!replaced) {
      buffer[j++] = str[i++];
    }
  }
  buffer[j] = '\0';
  strcpy(str, buffer);
}

void remove_accents(char *str) {
  const char accents[] =
      "ÀÁÂÃÄÅàáâãäåÈÉÊËèéêëÌÍÎÏìíîïÒÓÔÕÖØòóôõöøÙÚÛÜùúûüÇçÂÊÎÔÛâêîôû";
  const char replacements[] =
      "AAAAAAaaaaaaEEEEeeeeIIIIiiiiOOOOOOooooooUUUUuuuuCcAEIOUaeiou";

  for (int i = 0; str[i]; i++) {
    for (int j = 0; accents[j]; j++) {
      if ((unsigned char)str[i] == (unsigned char)accents[j]) {
        str[i] = replacements[j];
        break;
      }
    }
  }
}

void print_formatted_datetime(const char *iso) {
  struct tm tm;
  time_t t;
  char out[64];
  char buf[32];
  const char *p = iso;

  // Copier une partie utile sans fractions de seconde (avant .) et sans Z
  if (strchr(iso, '.')) {
    // Copier jusqu'au point avant la fraction
    size_t len = strchr(iso, '.') - iso;
    if (iso[len + 1] == 'Z')
      strncpy(buf, iso, len);
    else
      strncpy(buf, iso, len);
    buf[len] = '\0';
    p = buf;
  } else {
    // Pas de fraction, enlever le Z si présent
    size_t len = strlen(iso);
    if (iso[len - 1] == 'Z') {
      strncpy(buf, iso, len - 1);
      buf[len - 1] = '\0';
      p = buf;
    }
  }

  memset(&tm, 0, sizeof(tm));
  if (strptime(p, "%Y-%m-%dT%H:%M:%S", &tm) == NULL) {
    printf("%s\n", iso); // pas parsé => afficher brut
    return;
  }

  // Convertir temps UTC en time_t
  t = timegm(&tm); // timegm gère le temps en UTC, pas mktime

  // Convertir en temps local pour affichage
  struct tm *local = localtime(&t);
  if (!local) {
    printf("Erreur conversion temps\n");
    return;
  }

  strftime(out, sizeof(out), "%d/%m/%Y %H:%M", local);
  printf("%s\n", out);
}
