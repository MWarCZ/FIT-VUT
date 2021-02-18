/**
 * Kostra programu pro 3. projekt IZP 2015/16
 *
 * Jednoducha shlukova analyza
 * Complete linkage
 * http://is.muni.cz/th/172767/fi_b/5739129/web/web/clsrov.html
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h> // sqrtf
#include <limits.h> // INT_MAX
// Přidal student -/\/\-\/\/- 
#include <float.h> // MIN a MAX
#include <string.h> // strcmp()

/*****************************************************************
 * Ladici makra. Vypnout jejich efekt lze definici makra
 * NDEBUG, napr.:
 *   a) pri prekladu argumentem prekladaci -DNDEBUG
 *   b) v souboru (na radek pred #include <assert.h>
 *      #define NDEBUG
 */
#ifdef NDEBUG
#define debug(s)
#define dfmt(s, ...)
#define dint(i)
#define dfloat(f)
#else

// vypise ladici retezec
#define debug(s) printf("- %s\n", s)

// vypise formatovany ladici vystup - pouziti podobne jako printf
#define dfmt(s, ...) printf(" - "__FILE__":%u: "s"\n",__LINE__,__VA_ARGS__)

// vypise ladici informaci o promenne - pouziti dint(identifikator_promenne)
#define dint(i) printf(" - " __FILE__ ":%u: " #i " = %d\n", __LINE__, i)

// vypise ladici informaci o promenne typu float - pouziti
// dfloat(identifikator_promenne)
#define dfloat(f) printf(" - " __FILE__ ":%u: " #f " = %g\n", __LINE__, f)

#endif

/*****************************************************************
 * Deklarace potrebnych datovych typu:
 *
 * TYTO DEKLARACE NEMENTE
 *
 *   struct obj_t - struktura objektu: identifikator a souradnice
 *   struct cluster_t - shluk objektu:
 *      pocet objektu ve shluku,
 *      kapacita shluku (pocet objektu, pro ktere je rezervovano
 *          misto v poli),
 *      ukazatel na pole shluku.
 */

struct obj_t {
    int id;
    float x;
    float y;
};

struct cluster_t {
    int size;
    int capacity;
    struct obj_t *obj;
};

/*****************************************************************
 * Deklarace potrebnych funkci.
 *
 * PROTOTYPY FUNKCI NEMENTE
 *
 * IMPLEMENTUJTE POUZE FUNKCE NA MISTECH OZNACENYCH 'TODO'
 *
 */

/*
 Inicializace shluku 'c'. Alokuje pamet pro cap objektu (kapacitu).
 Ukazatel NULL u pole objektu znamena kapacitu 0.
*/
void init_cluster(struct cluster_t *c, int cap)
{
    assert(c != NULL);
    assert(cap >= 0);

    // TODO
    c->size = 0;
    c->obj = malloc(sizeof(struct obj_t) * cap);
    if(c->obj == NULL)
    {
        c->capacity = 0;
        return;
    }
    c->capacity = cap;
    return;
}

/*
 Odstraneni vsech objektu shluku a inicializace na prazdny shluk.
 */
void clear_cluster(struct cluster_t *c)
{
    // TODO
    free(c->obj);
    c->obj = NULL;
    c->size = 0;
    c->capacity = 0;
}

/// Chunk of cluster objects. Value recommended for reallocation.
const int CLUSTER_CHUNK = 10;

/*
 Zmena kapacity shluku 'c' na kapacitu 'new_cap'.
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap)
{
    // TUTO FUNKCI NEMENTE
    assert(c);
    assert(c->capacity >= 0);
    assert(new_cap >= 0);

    if (c->capacity >= new_cap)
        return c;

	 int newCap = (new_cap < CLUSTER_CHUNK)? CLUSTER_CHUNK : new_cap;
    size_t size = sizeof(struct obj_t) * newCap;

    void *arr = realloc(c->obj, size);
    if (arr == NULL)
        return NULL;

    c->obj = arr;
    c->capacity = new_cap;
    return c;
}

/*
 Prida objekt 'obj' na konec shluku 'c'. Rozsiri shluk, pokud se do nej objekt
 nevejde.
 */
void append_cluster(struct cluster_t *c, struct obj_t obj)
{
    // TODO
    int size = c->size;

    if(size >= c->capacity)
    {
        struct cluster_t *tmp = resize_cluster(c, size + CLUSTER_CHUNK);
        if(tmp == NULL)
        {
            return;
        }
        c = tmp;
    }

    c->obj[size] = obj;
    c->size++;
    
}

/*
 Seradi objekty ve shluku 'c' vzestupne podle jejich identifikacniho cisla.
 */
void sort_cluster(struct cluster_t *c);

/*
 Do shluku 'c1' prida objekty 'c2'. Shluk 'c1' bude v pripade nutnosti rozsiren.
 Objekty ve shluku 'c1' budou serazny vzestupne podle identifikacniho cisla.
 Shluk 'c2' bude nezmenen.
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c2 != NULL);

    // TODO
    int sizec1 = c1->size;
    int sizec2 = c2->size;
    if((sizec1 + sizec2) > c1->capacity)
    {
        struct cluster_t *tmp = resize_cluster(c1, sizec1 + sizec2);
        if(tmp == NULL)
        {
            return;
        }

    }
    
    for(int i = 0; i < sizec2; i++)
    {
        c1->obj[sizec1 + i] = c2->obj[i];
    }
    c1->size = sizec1 + sizec2;

	 sort_cluster(c1);

}

/**********************************************************************/
/* Prace s polem shluku */

/*
 Odstrani shluk z pole shluku 'carr'. Pole shluku obsahuje 'narr' polozek
 (shluku). Shluk pro odstraneni se nachazi na indexu 'idx'. Funkce vraci novy
 pocet shluku v poli.
*/
int remove_cluster(struct cluster_t *carr, int narr, int idx)
{
    assert(idx < narr);
    assert(narr > 0);

    // TODO
	
	clear_cluster(&carr[idx]);
	for(int i = idx; i < narr-1; i++)
	{
		carr[i] = carr[i+1];
	}

	return narr-1;
}

/*
 Pocita Euklidovskou vzdalenost mezi dvema objekty.
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2)
{
    assert(o1 != NULL);
    assert(o2 != NULL);

    // TODO
	float tmpX = o1->x - o2->x;
	float tmpY = o1->y - o2->y;
	float tmpEnd = sqrtf( tmpX*tmpX + tmpY*tmpY );
	return tmpEnd;
}

/*
 Pocita vzdalenost dvou shluku.
*/
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c1->size > 0);
    assert(c2 != NULL);
    assert(c2->size > 0);

    // TODO
	float max = FLT_MIN;
   for(int i = 0; i < c1->size; i++)
   {
      for(int j = 0; j < c2->size; j++)
		{
         float tmpMax = obj_distance(&c1->obj[i], &c2->obj[j]);
         if(tmpMax > max)
         {
				max = tmpMax;
			}
		}
	}
	return max;
}

/*
 Funkce najde dva nejblizsi shluky. V poli shluku 'carr' o velikosti 'narr'
 hleda dva nejblizsi shluky. Nalezene shluky identifikuje jejich indexy v poli
 'carr'. Funkce nalezene shluky (indexy do pole 'carr') uklada do pameti na
 adresu 'c1' resp. 'c2'.
*/
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2)
{
    assert(narr > 0);

    // TODO
	float min = FLT_MAX;
    for(int i = 0; i < narr; i++)
    {
        for(int j = i+1; j < narr; j++)
        {
            float tmpMin = cluster_distance(&carr[i], &carr[j]);
            if(tmpMin < min)
            {
                min = tmpMin;
                *c1 = i;
                *c2 = j;
            }
        }
    }
}

// pomocna funkce pro razeni shluku
static int obj_sort_compar(const void *a, const void *b)
{
    // TUTO FUNKCI NEMENTE
    const struct obj_t *o1 = a;
    const struct obj_t *o2 = b;
    if (o1->id < o2->id) return -1;
    if (o1->id > o2->id) return 1;
    return 0;
}

/*
 Razeni objektu ve shluku vzestupne podle jejich identifikatoru.
*/
void sort_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    qsort(c->obj, c->size, sizeof(struct obj_t), &obj_sort_compar);
}

/*
 Tisk shluku 'c' na stdout.
*/
void print_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    for (int i = 0; i < c->size; i++)
    {
        if (i) putchar(' ');
        printf("%d[%g,%g]", c->obj[i].id, c->obj[i].x, c->obj[i].y);
    }
    putchar('\n');
}


/*
 Ze souboru 'filename' nacte objekty. Pro kazdy objekt vytvori shluk a ulozi
 jej do pole shluku. Alokuje prostor pro pole vsech shluku a ukazatel na prvni
 polozku pole (ukalazatel na prvni shluk v alokovanem poli) ulozi do pameti,
 kam se odkazuje parametr 'arr'. Funkce vraci pocet nactenych objektu (shluku).
 V pripade nejake chyby uklada do pameti, kam se odkazuje 'arr', hodnotu NULL.
*/
int load_clusters(char *filename, struct cluster_t **arr)
{
    assert(arr != NULL);

    // TODO	

	FILE *file = fopen(filename, "r");
   if(file == NULL)
   {
        *arr = NULL;
		  fprintf(stderr,"Soubor se nepodařilo otevřít.\n");
        return -1;
   }

   int c = 0, count = 0;
    
  // c = fscanf(file,"%50s\n",str);
   c = fscanf(file,"count=%d",&count);


   if ( !c )
   {
		count = 0;
		*arr = NULL;
		fprintf(stderr,"Soubor neobsahuje údaj o počtu uložených objektů.\n");
   }

   if(count > 0)
   {
     struct cluster_t *tmp = malloc(sizeof(struct cluster_t) * count);
	  int row = 2;
     for(int i = 0; i < count; i++, row++)
     {
         int loadId = 0;
			float loadX = 0.0, loadY = 0.0;

         //c = fscanf(file,"%i %f %f %*[^\n]",&loadId,&loadX,&loadY);
 			c = fscanf(file,"%i %f %f\n",&loadId,&loadX,&loadY);
			
			for(int ii = 0; ii < i; ii++)
			{
				if(tmp[ii].obj[0].id == loadId)
				{
					c = 0;
					break;
				}
			}

         if(c == 3 && (float)(int)loadX == loadX && (float)(int)loadY == loadY && loadX >= 0 && loadX <= 1000 && loadY >= 0 && loadY <= 1000)
         {
				init_cluster( &tmp[i] ,1);

            struct obj_t obj;
            obj.id = loadId;
            obj.x = loadX;
            obj.y = loadY; 
            append_cluster(&tmp[i],obj);
         }
         else
         {
            for(int ii = 0; ii < i; ii++)
				{
					clear_cluster(&tmp[ii]);
				}
				free(tmp);
				tmp = NULL;
				count = 0;
				/*
				fscanf(file,"%*s\n");
				count--;
				i--;
				printf("Data na řádeku číslo %i jsou v neplatném formátu.\n",row);
				*/
				fprintf(stderr,"Litujeme soubor obsahuje neplatná data. \n");
         }
     }
		*arr = tmp;
   }

   
	if(fclose(file) == EOF)
   {
        fprintf(stderr,"Soubor se nepodařilo zavřít, \nale my nejsme svině a dovolíme vám pokračovat v práci s programem.\n");
   }
   return count;
}

/*
 Tisk pole shluku. Parametr 'carr' je ukazatel na prvni polozku (shluk).
 Tiskne se prvnich 'narr' shluku.
*/
void print_clusters(struct cluster_t *carr, int narr)
{
    printf("Clusters:\n");
    for (int i = 0; i < narr; i++)
    {
        printf("cluster %d: ", i);
        print_cluster(&carr[i]);
    }
}

int main(int argc, char *argv[])
{
    struct cluster_t *clusters;

    // TODO
	(void)argc; (void)argv;
/*	struct cluster_t cl, cl2;
	init_cluster(&cl, 2);
	init_cluster(&cl2, 10);
	
	printf("size=%i  capacity=%i \n",cl.size,cl.capacity);
	

	struct obj_t obj;
	obj.id = 10;
	obj.x = 5;
	obj.y = 6;
	append_cluster(&cl,obj);
	append_cluster(&cl,obj);
	append_cluster(&cl,obj);
	append_cluster(&cl,obj);
	append_cluster(&cl,obj);

	for(int i = 0; i < 9; i++)
	{
		obj.id = i;
		obj.x = 5 * i;
		obj.y = 3+i *2;
		append_cluster(&cl2,obj);
	}

	printf("CL: size=%i  capacity=%i \n",cl.size,cl.capacity);
	print_cluster(&cl);
	putchar('\n');
	printf("CL2: size=%i  capacity=%i \n",cl2.size,cl2.capacity);
	print_cluster(&cl2);
	putchar("\n");
	merge_clusters(&cl,&cl2);
	printf("CL: size=%i  capacity=%i \n",cl.size,cl.capacity);
	print_cluster(&cl);



	clear_cluster(&cl2);
	clear_cluster(&cl);
*/

	char *fileName = "";
	int finalCount = 1;
	if(argc > 1)
	{
		fileName = argv[1];
	
		if(argc > 2)
		{
			finalCount = atoi(argv[2]);
			if(finalCount<1)
			{
				fprintf(stderr,"Byl zadán neplatný počet shluků.\n");
				return 0;
			}
			if(argc > 3)
			{
				fprintf(stderr,"Bylo zadáno příliž mnoho parametrů.\n");
				return 0;
			}

		}

	
		int count = load_clusters(fileName,&clusters);

		if(clusters != NULL)
		{
			if(count >= finalCount)
			{
				//finalCount = (finalCount < 0)? count : finalCount;
				while(count > finalCount)
				{
					int index1=-1, index2=-1;
					find_neighbours(clusters, count, &index1, &index2);

					if(index1 != index2)
					{
						merge_clusters(&clusters[index1],&clusters[index2]);

						count = remove_cluster(clusters, count, index2);
					}
				}

				for(int i = 0; i < count; i++)
				{
					sort_cluster(&clusters[i]);
				}

				print_clusters(clusters,count);
		
			}
			else
			{
				fprintf(stderr,"Zadnaný počet shluků je větší než načtený počet shluků.\n");
			}
			for(int i = 0; i < count; i++)
			{
				clear_cluster(&clusters[i]);

			}
			free(clusters);
		}
		else
		{
			/*
			for(;;)
				fprintf(stderr,"Chyba!!! ");
			*/
		}
	}

	return 0;
}







