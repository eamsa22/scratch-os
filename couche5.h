/**
 * @file couche5.c
 * @brief Couche permettant de réaliser l'implémentation du terminal et des différentes
 */

#ifndef __COUCHE5_H__
#define __COUCHE5_H__

/**
 * @brief fonction qui lance l'interpreteur de  commande --> return -1 en cas de probleme
 * @return int
 */
int lancer_interprete_cmd();

/**
 * @brief fonction qui verifie si on peut demarer l'interprete de commandes --> renvoie 0  sinon -1
 * @return int
 */
int verif_demarer_interprete_cmd();

/**
 * @brief fonction qui verifie si le login et password correctes ->renvoie -1 si non trouvé sinon 0 et stocke dans session le id du user
 * @return int
 */
int verifier_user();

/**
fonction pour executer la commande cmd -> renvoie -1  en cas de probleme sinon 0
*/
int executer_cmd(cmd_t cmd);

/**
 * @brief fonction decoupe...
 * @param Chaine tableau de characteres source
 * @param pMots tableau de chaine de caracteres destination
 * @return int
 */
int Decoupe(char Chaine[], char *pMots[]);

#endif
