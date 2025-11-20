/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search_func.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ascordil <ascordil@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 00:49:41 by ascordil          #+#    #+#             */
/*   Updated: 2025/05/21 22:47:32 by ascordil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ratp.h"

void print_last_passages_clichy_ligne_14(const char *json) {
  cJSON *root = cJSON_Parse(json);
  if (!root) {
    printf("Erreur de parsing JSON\n");
    return;
  }

  cJSON *siri = cJSON_GetObjectItem(root, "Siri");
  cJSON *service = siri ? cJSON_GetObjectItem(siri, "ServiceDelivery") : NULL;
  cJSON *stopMonitoring =
      service ? cJSON_GetObjectItem(service, "StopMonitoringDelivery") : NULL;

  if (!stopMonitoring || !cJSON_IsArray(stopMonitoring) ||
      cJSON_GetArraySize(stopMonitoring) == 0) {
    printf("Aucun passage imminent trouvé.\n");
    cJSON_Delete(root);
    return;
  }

  // Variables pour mémoriser les derniers passages par direction
  cJSON *lastVisitSDP = NULL; // Saint-Denis - Pleyel
  cJSON *lastVisitADO = NULL; // Aéroport d'Orly
  char *lastTimeSDP = NULL;
  char *lastTimeADO = NULL;

  int deliveriesCount = cJSON_GetArraySize(stopMonitoring);
  for (int i = 0; i < deliveriesCount; i++) {
    cJSON *delivery = cJSON_GetArrayItem(stopMonitoring, i);
    cJSON *visits =
        delivery ? cJSON_GetObjectItem(delivery, "MonitoredStopVisit") : NULL;
    if (!visits || !cJSON_IsArray(visits))
      continue;

    int visitCount = cJSON_GetArraySize(visits);
    for (int j = 0; j < visitCount; j++) {
      cJSON *visit = cJSON_GetArrayItem(visits, j);
      cJSON *journey =
          visit ? cJSON_GetObjectItem(visit, "MonitoredVehicleJourney") : NULL;
      if (!journey)
        continue;

      cJSON *lineRef = cJSON_GetObjectItem(journey, "LineRef");
      cJSON *line_val = lineRef ? cJSON_GetObjectItem(lineRef, "value") : NULL;
      if (!line_val || !line_val->valuestring)
        continue;

      if (strcmp(line_val->valuestring, "STIF:Line::C01384:") != 0) // Ligne 14
        continue;

      cJSON *call = cJSON_GetObjectItem(journey, "MonitoredCall");
      if (!call)
        continue;
      cJSON *stopPoints = cJSON_GetObjectItem(call, "StopPointName");
      if (!stopPoints || !cJSON_IsArray(stopPoints))
        continue;
      cJSON *first_stop = cJSON_GetArrayItem(stopPoints, 0);
      cJSON *stop_val =
          first_stop ? cJSON_GetObjectItem(first_stop, "value") : NULL;
      if (!stop_val || !stop_val->valuestring ||
          strcmp(stop_val->valuestring, "Porte de Clichy") != 0)
        continue;

      cJSON *dest_array = cJSON_GetObjectItem(journey, "DirectionName");
      if (!dest_array || !cJSON_IsArray(dest_array))
        continue;
      cJSON *first_dest = cJSON_GetArrayItem(dest_array, 0);
      cJSON *val = first_dest ? cJSON_GetObjectItem(first_dest, "value") : NULL;
      if (!val || !val->valuestring)
        continue;

      char *direction = val->valuestring;
      if (strcmp(direction, "Saint-Denis - Pleyel") != 0 &&
          strcmp(direction, "Aéroport d'Orly") != 0)
        continue;

      cJSON *expected = cJSON_GetObjectItem(call, "ExpectedDepartureTime");
      if (!expected || !expected->valuestring)
        continue;

      // Comparer les timestamp pour stocker le dernier
      if (strcmp(direction, "Saint-Denis - Pleyel") == 0) {
        if (!lastTimeSDP || strcmp(expected->valuestring, lastTimeSDP) > 0) {
          lastTimeSDP = expected->valuestring;
          lastVisitSDP = visit;
        }
      } else if (strcmp(direction, "Aéroport d'Orly") == 0) {
        if (!lastTimeADO || strcmp(expected->valuestring, lastTimeADO) > 0) {
          lastTimeADO = expected->valuestring;
          lastVisitADO = visit;
        }
      }
    }
  }

  printf(COLOR_YELLOW "─────────────────────── LIGNE 14 LAST STOPS "
                      "─────────────────────────\n" COLOR_RESET);
  if (lastVisitSDP) {
    cJSON *journey =
        cJSON_GetObjectItem(lastVisitSDP, "MonitoredVehicleJourney");
    cJSON *call = cJSON_GetObjectItem(journey, "MonitoredCall");
    cJSON *expected = cJSON_GetObjectItem(call, "ExpectedDepartureTime");

    printf(COLOR_GREEN "►" COLOR_RESET " Direction :" COLOR_GREEN
                       " %-25s- " COLOR_RESET "Arrivée : ",
           "Saint-Denis - Pleyel");

    print_formatted_time_only_colored(expected->valuestring, COLOR_GREEN);
    printf("\n");
  } else {
    printf("  Aucun passage trouvé pour Saint-Denis - Pleyel.\n\n");
  }
  printf(COLOR_YELLOW
         "───────────────────────────────────────────────────────────────────"
         "──\n" COLOR_RESET);

  if (lastVisitADO) {
    cJSON *journey =
        cJSON_GetObjectItem(lastVisitADO, "MonitoredVehicleJourney");
    cJSON *call = cJSON_GetObjectItem(journey, "MonitoredCall");
    cJSON *expected = cJSON_GetObjectItem(call, "ExpectedDepartureTime");

    printf(COLOR_BLUE "►" COLOR_RESET " Direction :" COLOR_BLUE
                      " %-25s - " COLOR_RESET "Arrivée : ",
           "Aéroport d'Orly");

    print_formatted_time_only_colored(expected->valuestring, COLOR_BLUE);
    printf("\n");
  } else {
    printf("  Aucun passage trouvé pour Aéroport d'Orly.\n\n");
  }

  printf(COLOR_YELLOW
         "───────────────────────────────────────────────────────────────────"
         "──\n" COLOR_RESET);
  cJSON_Delete(root);
}

void print_next_passages_clichy_ligne_14(const char *json) {
  cJSON *root = cJSON_Parse(json);

  if (!root) {
    printf("Erreur de parsing JSON\n");
    return;
  }

  cJSON *siri = cJSON_GetObjectItem(root, "Siri");
  cJSON *service = siri ? cJSON_GetObjectItem(siri, "ServiceDelivery") : NULL;
  cJSON *stopMonitoring =
      service ? cJSON_GetObjectItem(service, "StopMonitoringDelivery") : NULL;
  cJSON *firstDelivery = (stopMonitoring && cJSON_IsArray(stopMonitoring))
                             ? cJSON_GetArrayItem(stopMonitoring, 0)
                             : NULL;
  cJSON *visits = firstDelivery
                      ? cJSON_GetObjectItem(firstDelivery, "MonitoredStopVisit")
                      : NULL;

  if (!visits || !cJSON_IsArray(visits) || cJSON_GetArraySize(visits) == 0) {
    printf("Aucun passage imminent trouvé.\n");
    cJSON_Delete(root);
    return;
  }

  int n = cJSON_GetArraySize(visits);
  int count = 0; // Nombre de passages affichés

  printf(COLOR_YELLOW "─────────────────────── LIGNE 14 NEXT STOPS "
                      "─────────────────────────\n" COLOR_RESET);
  for (int i = 0; i < n; ++i) {
    cJSON *visit = cJSON_GetArrayItem(visits, i);
    if (!visit)
      continue;

    cJSON *journey = cJSON_GetObjectItem(visit, "MonitoredVehicleJourney");
    if (!journey)
      continue;

    // Ligne
    cJSON *lineRef = cJSON_GetObjectItem(journey, "LineRef");
    cJSON *line_val = lineRef ? cJSON_GetObjectItem(lineRef, "value") : NULL;
    if (!line_val || !line_val->valuestring)
      continue;

    // Arrêt
    cJSON *call = cJSON_GetObjectItem(journey, "MonitoredCall");
    if (!call)
      continue;
    cJSON *stopPoints = cJSON_GetObjectItem(call, "StopPointName");
    if (!stopPoints || !cJSON_IsArray(stopPoints))
      continue;
    cJSON *first_stop = cJSON_GetArrayItem(stopPoints, 0);
    cJSON *stop_val =
        first_stop ? cJSON_GetObjectItem(first_stop, "value") : NULL;
    if (!stop_val || !stop_val->valuestring)
      continue;

    if (strcmp(stop_val->valuestring, "Porte de Clichy") != 0)
      continue;

    // Direction
    cJSON *dest_array = cJSON_GetObjectItem(journey, "DirectionName");
    const char *direction = "Inconnu";
    if (dest_array && cJSON_IsArray(dest_array)) {
      cJSON *first_dest = cJSON_GetArrayItem(dest_array, 0);
      if (first_dest) {
        cJSON *val = cJSON_GetObjectItem(first_dest, "value");
        if (val && val->valuestring) {
          direction = val->valuestring;
        }
      }
    }

    // Filtre sur les destinations officielles selon la ligne
    int keep = 0;
    if (strcmp(direction, "Saint-Denis - Pleyel") == 0 ||
        strcmp(direction, "Aéroport d'Orly") == 0) {
      keep = 1;
    }
    if (!keep)
      continue;

    cJSON *expected =
        call ? cJSON_GetObjectItem(call, "ExpectedDepartureTime") : NULL;
    if (!expected || !expected->valuestring)
      continue;

    if (strcmp(direction, "Saint-Denis - Pleyel") == 0) {

      printf(COLOR_GREEN "►" COLOR_RESET " Direction :" COLOR_GREEN
                         " %-25s- " COLOR_RESET "Arrivée : ",
             "Saint-Denis - Pleyel");

      print_formatted_time_only_colored(expected->valuestring, COLOR_GREEN);
      printf("\n");
      printf(
          COLOR_YELLOW
          "───────────────────────────────────────────────────────────────────"
          "──\n" COLOR_RESET);
    } else if (strcmp(direction, "Aéroport d'Orly") == 0) {

      printf(COLOR_BLUE "►" COLOR_RESET " Direction :" COLOR_BLUE
                        " %-25s - " COLOR_RESET "Arrivée : ",
             "Aéroport d'Orly");

      print_formatted_time_only_colored(expected->valuestring, COLOR_BLUE);
      printf("\n");
      printf(
          COLOR_YELLOW
          "───────────────────────────────────────────────────────────────────"
          "──\n" COLOR_RESET);
    } else {
      printf("  Ligne %d - Direction : %-25s - Passage à ", 14, direction);
      print_formatted_datetime(expected->valuestring);
      printf("\n\n");
    }

    count++;
  }

  if (count == 0)
    printf("Aucun passage imminent trouvé.\n");

  cJSON_Delete(root);
}

void print_all_next_passages_of_station(const char *json) {
  cJSON *root = NULL, *siri = NULL, *service = NULL, *stopMonitoring = NULL;
  cJSON *firstDelivery = NULL, *visits = NULL, *visit = NULL;
  int n = 0, i = 0;

  root = cJSON_Parse(json);
  if (!root) {
    printf("Erreur de parsing JSON\n");
    return;
  }

  siri = cJSON_GetObjectItem(root, "Siri");
  service = siri ? cJSON_GetObjectItem(siri, "ServiceDelivery") : NULL;
  stopMonitoring =
      service ? cJSON_GetObjectItem(service, "StopMonitoringDelivery") : NULL;
  firstDelivery = (stopMonitoring && cJSON_IsArray(stopMonitoring))
                      ? cJSON_GetArrayItem(stopMonitoring, 0)
                      : NULL;
  visits = firstDelivery
               ? cJSON_GetObjectItem(firstDelivery, "MonitoredStopVisit")
               : NULL;

  if (!visits || !cJSON_IsArray(visits) ||
      (n = cJSON_GetArraySize(visits)) == 0) {
    printf("Aucun passage imminent trouvé.\n");
    cJSON_Delete(root);
    return;
  }

  printf("Tous les prochains passages :\n");
  for (i = 0; i < n; ++i) {
    visit = cJSON_GetArrayItem(visits, i);
    if (!visit)
      continue;

    cJSON *journey = cJSON_GetObjectItem(visit, "MonitoredVehicleJourney");
    cJSON *call =
        journey ? cJSON_GetObjectItem(journey, "MonitoredCall") : NULL;
    cJSON *expected =
        call ? cJSON_GetObjectItem(call, "ExpectedArrivalTime") : NULL;

    cJSON *dest_array =
        journey ? cJSON_GetObjectItem(journey, "DirectionName") : NULL;
    const char *direction = "Inconnu";
    if (dest_array && cJSON_IsArray(dest_array)) {
      cJSON *first_dest = cJSON_GetArrayItem(dest_array, 0);
      if (first_dest) {
        cJSON *val = cJSON_GetObjectItem(first_dest, "value");
        if (val && val->valuestring) {
          direction = val->valuestring;
        }
      }
    }

    if (expected && expected->valuestring) {
      printf("  Direction : %-25s - Passage à ", direction);
      print_formatted_datetime(expected->valuestring);
    }
  }

  cJSON_Delete(root);
}
