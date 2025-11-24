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

void print_ascii_tag(void) {
  printf(COLOR_GREEN);
  printf("                      888                   \n");
  printf("                     888                    \n");
  printf("                     888                    \n");
  printf("88888b.d88b.  .d88b. 888888888d888 .d88b.  \n");
  printf("888 \"888 \"88bd8P  Y8b888   888P\"  d88\"\"88b \n");
  printf("888  888  88888888888888   888    888  888 \n");
  printf("888  888  888Y8b.    Y88b. 888    Y88..88P \n");
  printf("888  888  888 \"Y8888  \"Y888888     \"Y88P\"  \n");
  printf(COLOR_RESET);
  printf("\n");
}

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

  // filtrer la bonne partie avant (.) et sans Z
  if (strchr(iso, '.')) {
    // copier la parti avant
    size_t len = strchr(iso, '.') - iso;
    if (iso[len + 1] == 'Z')
      strncpy(buf, iso, len);
    else
      strncpy(buf, iso, len);
    buf[len] = '\0';
    p = buf;
  } else {
    // si pas de fraction, retirer le Z si present
    size_t len = strlen(iso);
    if (iso[len - 1] == 'Z') {
      strncpy(buf, iso, len - 1);
      buf[len - 1] = '\0';
      p = buf;
    }
  }

  memset(&tm, 0, sizeof(tm));
  if (strptime(p, "%Y-%m-%dT%H:%M:%S", &tm) == NULL) {
    printf("%s\n", iso);
    return;
  }

  // converssion temps UTC en time_t
  t = timegm(&tm);

  // conversion en temps local
  struct tm *local = localtime(&t);
  if (!local) {
    printf("Erreur conversion temps\n");
    return;
  }

  strftime(out, sizeof(out), "%d/%m/%Y %H:%M", local);
  printf("%s\n", out);
}

void print_formatted_time_only(const char *iso) {
  struct tm tm;
  time_t t;
  char out[64];
  char buf[32];
  const char *p = iso;

  if (strchr(iso, '.')) {
    size_t len = strchr(iso, '.') - iso;
    if (iso[len + 1] == 'Z')
      strncpy(buf, iso, len);
    else
      strncpy(buf, iso, len);
    buf[len] = '\0';
    p = buf;
  } else {
    size_t len = strlen(iso);
    if (iso[len - 1] == 'Z') {
      strncpy(buf, iso, len - 1);
      buf[len - 1] = '\0';
      p = buf;
    }
  }

  memset(&tm, 0, sizeof(tm));
  if (strptime(p, "%Y-%m-%dT%H:%M:%S", &tm) == NULL) {
    printf("%s\n", iso);
    return;
  }
  t = timegm(&tm);
  struct tm *local = localtime(&t);
  if (!local) {
    printf("Erreur conversion temps\n");
    return;
  }
  // format heure:minute
  strftime(out, sizeof(out), "%H:%M", local);
  printf("%s", out);
}

void print_formatted_time_only_colored(const char *iso, const char *color) {
  struct tm tm;
  time_t t;
  char out[64];
  char buf[32];
  const char *p = iso;

  if (strchr(iso, '.')) {
    size_t len = strchr(iso, '.') - iso;
    if (iso[len + 1] == 'Z')
      strncpy(buf, iso, len);
    else
      strncpy(buf, iso, len);
    buf[len] = '\0';
    p = buf;
  } else {
    size_t len = strlen(iso);
    if (iso[len - 1] == 'Z') {
      strncpy(buf, iso, len - 1);
      buf[len - 1] = '\0';
      p = buf;
    }
  }
  memset(&tm, 0, sizeof(tm));
  if (strptime(p, "%Y-%m-%dT%H:%M:%S", &tm) == NULL) {
    printf("%s\n", iso);
    return;
  }
  t = timegm(&tm);
  struct tm *local = localtime(&t);
  if (!local) {
    printf("Erreur conversion temps\n");
    return;
  }
  strftime(out, sizeof(out), "%H:%M", local);
  printf("%s%s%s", color, out, COLOR_RESET);
}
