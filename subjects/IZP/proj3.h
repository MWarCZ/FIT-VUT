#ifndef PROJ3_H
#define PROJ3_H

/**
 * Kostra hlavickoveho souboru 3. projekt IZP 2015/16
 * a pro dokumentaci Javadoc.
 */

/**
 *  @defgroup structs1 Structs in project.
 *  @{
 */


/**
 *  @brief 
 *  Struct with coordiantes and identifier.
 */
struct obj_t {
    int id; /**< obj_t identifier.*/
    float x; /**< Coordinates X.*/
    float y; /**< Coordinates Y.*/
};

/** 
 *  @brief
 *  Struct with size, capacity and array of obj_t.
 */
struct cluster_t {
    int size; /**< Size of loaded obj_t in @b obj. */
    int capacity; /**< Size of allocated array @b obj. */
    struct obj_t *obj; /**< Array of obj_t. */
};

/**
 *  @}
 */

/**
 *  @defgroup functions1 Functions in project.
 *  @{
 */


/** 
 *  @brief Function initializes cluster_t c to default values.
 *  
 *  @param[out] c Pointer to cluster_t for initializes.
 *  @param[in] cap Number specifying capacity of array cluster_t::obj.
 *  @pre 
 *  @b c != NULL @n
 *	@b cap >= 0
 *  
 *  @post 
 *  @b c is inicialize. @n 
 *  (cluster_t::size = 0 and if If allocated array cluster_t::obj was successful, than set cluster_t::capacity = @b cap, else cluster_t::capacity = 0 and cluster_t::obj = NULL)
 */
void init_cluster(struct cluster_t *c, int cap);

/**
 *  @brief Function free cluster_t::obj in @b c and set cluster_t::size and cluster_t::capacity to default value.
 *  @param[out] c Pointer to cluster_t for clear.
 *  @b c != NULL.
 *  @post
 *  In @b c are cluster_t::size = 0, cluster_t::capacity = 0 and cluster_t::obj == NULLL.
 */
void clear_cluster(struct cluster_t *c);

/**
 *  @var CLUSTER_CHUNK
 *  Number chunk of cluster objects. Value recommended for reallocation.
 */
extern const int CLUSTER_CHUNK;

/**
 *  @brief Function reallocate cluster_t::obj to new size.
 *  @param[out] c Pointer to cluster_t with cluster_t::obj for ralocate.
 *  @param[in] new_cap Number of new capacity.
 *  @return Returned pointer to cluster_t @b c or NULL if reallocated failed.
 *  @pre
 *  @b c != NULL. @n
 *  In @b c cluster_t::capacity >= 0 @n
 *  @b new_cap >= 0
 *  @post
 *  If @b new_cap <= cluster_t::capacity in @b c then array cluster_t::obj is not reallocated. @n
 *  Else array cluster_t::obj is reallocated to new size and cluster_t::capacity = @b new_cap.
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap);

/**
 *  @brief Function add new obj_t @b obj to cluster_t @b c.
 *  @param[out] c Pointer to cluster_t where @b obj will be insered.
 *  @param[in] obj New obj_t will be inserted to sturct_t::obj in @b c.
 */
void append_cluster(struct cluster_t *c, struct obj_t obj);

/**
 *  @brief Function add all obj_t from @b c2 to @b c1.
 *  @param[out] c1 Pointer to cluster_t is destination.
 *  @param[in] c2 Pointer to cluster_t is source.
 *  @pre
 *  @b c1 != NULL. @n
 *  @b c2 != NULL.
 *  
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2);

/**
 *  @brief Function remove cluster_t from array of cluster_t @b carr on position @b idx.
 *  @param carr Array of cluster_t, where is cluster_t to remove.
 *  @param[in] narr Number count array.
 *  @param[in] idx Number of position cluster_t for remove in array @b carr.
 *  @return Returned new size array @b carr.
 *  @pre 
 *  @b idx < @b  narr. @n
 *  @b narr > 0;
 *  
 */
int remove_cluster(struct cluster_t *carr, int narr, int idx);

/**
 *  @brief Function calculates distance between two obj_t.
 *  @param[in] o1 Pointer to obj_t.
 *  @param[in] o2 Pointer to obj_t.
 *  @return Returned Euclidean distance between @b o1 and @b o2.
 *  @pre
 *  @b o1 != NULL. @n
 *  @b o2 != NULL.
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2);

/**
 *  @brief Funclion calculates distance between two cluster_t.
 *  @param[in] c1 Pointer to cluster_t.
 *  @param[in] c2 Pointer to cluster_t.
 *  @return Returned distance between @b c1 and @b c2.
 *  @pre
 *  @b c1 != NULL. @n
 *  @b c2 != NULL. @n
 *  In @b c1 cluster_t::size > 0 @n
 *  In @b c2 cluster_t::size > 0
 */
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2);

/**
 *  @brief Function searches for the two nearest cluster_t.
 *  @param carr[in] Array of cluster_t, where is cluster_t to remove.
 *  @param narr[in] Number count array.
 *  @param c1[out] Pointer to int for return position first cluster_t from @b carr
 *  @param c2[out] Pointer to int for return position second cluster_t form @b carr
 *  @pre
 *  @b narr > 0
 */
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2);

/**
 *  @brief Function sort array cluster_t::obj in @b c. 
 *  @param c Pointer to cluster_t, where is array of obj_t.
 */
void sort_cluster(struct cluster_t *c);

/**
 *  @brief Function print all obj_t from array cluster_t::obj in @b c on @c stdout.
 *  @param[in] c Pointer to cluster_t, where is array of obj_t.
 */
void print_cluster(struct cluster_t *c);

/**
 *  @brief Function load obj_t form file and add obj_t to new cluster_t in @b arr.
 *  @param[in] filename String filename.
 *  @param[out] arr Pointer to array of cluster_t.
 *  @return Returned count array of cluster_t or -1 if file not exist.
 *  @pre
 *  @b arr != NULL @n
 *  Data in file must follow format.
 *  First line with format @"@c count=3@" is count of obj_t in file and other lines are obj_t and must follow format @"obj_t::id obj_t::x obj_t::y@".
 *  @post
 *  @b *arr is NULL if something is wrong else @b *arr is address of allocated array.
 */
int load_clusters(char *filename, struct cluster_t **arr);

/**
 *  @brief Function print all cluster_t in array @b carr on @c stdout.
 *  @param[in] carr Array of cluster_t for print.
 *  @param[in] narr Number count array.
 */
void print_clusters(struct cluster_t *carr, int narr);
/**
 * @}
 */
#endif
