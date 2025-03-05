/**
 * @file couche2.c
 * @brief Gérer la table d'inode et le superblock, ce qui correspond aux informations pour gérer les fichiers
 */

#ifndef __COUCHE2__
#define __COUCHE2__
#define TOUT_VA_BIEN 0
#define ERREUR_ECRITURE 2
#define ERREUR_LECTURE 3

/**
 * @brief fonction qui écrit le super bloc au tout début du fichier disque
 * @return int =
 */
int write_super_block(void);

/**
 * @brief fonction qui lit le super bloc au tout début du disque
 * @return block_t
 */
int read_super_block(void);

/**
 * @brief ecrire la table d’inodes sur le systeme a la suite du super bloc
 */
int write_inodes_table(void);

/**
fonction permettant de charger la table d’inodes depuis le système --> retourne le nombre des inodes effectif
*/
int read_inodes_table(void);

/**
 * @brief
 */
void write_storage();

/**
 * @brief
 */
void read_storage();

/**
 * @brief fonction pour mettre à jour le champs first_free_byte du super bloc apres ecriture sur le disque
 * @param nb_blocs
 */
void update_first_free_byte(int nb_blocs);

/**
 * @brief fonction qui supprime un inode de la table des inodes et compacte la table
 * @param indice_inode
 */
void delete_inode(int indice_inode);


/**
 * @brief fonction qui retourn l'indice du premier inode disponible sinon -1
 * @return int
 */
int get_unused_inode();

/**
 * @brief fonction qui initialise un inode
 * @param inode
 * @param File_name
 * @param taille
 * @param pos
 */
void init_inode(inode_t *inode, char File_name[],uint taille,int pos);


/**
 * @brief Sert à modifier le superbloc sur le disque virtuel
 * @param
 * @return
 */
 int update_disk(char *tem_file);

/**
 *@brief
 *
 */

char *timestamp();

#endif
