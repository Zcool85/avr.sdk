// This file is part of ___PACKAGENAME___.
//
// ___PACKAGENAME___ is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// ___PACKAGENAME___ is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with ___PACKAGENAME___.  If not, see <http://www.gnu.org/licenses/>.
//
// ___COPYRIGHT___

/**
 * @file      ___FILENAME___
 *
 * @author    ___FULLUSERNAME___
 * @date      ___DATE___
 * @brief     Programme principal
 *
 * @details   Code source du progrmame principal.
 *
 * @copyright ___COPYRIGHT___
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/power.h>
#include <avr/wdt.h>

/**
 * @brief       Fonction d'initialisation du microcontroleur
 * @details     Cette fonction permet l'initialisation des différents éléments
 *              du microcontroller telles que ces entrées / sorties ou la
 *              gestion de l'alimentation
 */
void initialisation(void) {
    // Désactivation des interruptions
    cli();
    
    // gestion de l'alimentation
    power_all_enable();
    
    // Activation du chien de garde toutes les 500 ms
    wdt_enable(WDTO_500MS);
    
    // réactivation des interruptions
    sei();
}

/**
 * @brief       Point d'entrée principal du programme
 * @details     Fonction appelée au démarrage du microcontroleur
 */
int main(void) {
	initialisation();

    // Boucle infinie
	while (1) {
	}
    
	return 0; // Ne sera jamais atteint
}

/* EOF */