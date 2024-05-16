#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


// Fonctions utilitaires pour obtenir le max/min de deux nombres.
int max(int a, int b) { return (a > b) ? a : b; }
int min(int a, int b) { return (a < b) ? a : b; }
// Fonction d'échange de deux éléments
void swap(float* a, float* b) {
    float t = *a;
    *a = *b;
    *b = t;
}

// Définition de la structure pour les objets.
typedef struct {
    unsigned int weight;
    unsigned int value;
} Item;

void swapitem(Item* a, Item* b) {
    Item t = *a;
    *a = *b;
    *b = t;
}

int abs(int a) {
    return (a < 0) ? -1 * a : a;
}





// Variables globales pour l'algorithme de backtracking
// (ATTENTION: nous utilisons les variables globales dans cette version pour suivre le schéma vu en cours et en td)
Item* g_items;             // tableau d'objets
int g_n;                   // nombre d'objets
int g_W;            // capacité du sac
int g_bestValue = 0;       // meilleure valeur trouvée jusqu'à présent
int g_currentValue = 0;    // valeur courante du sac
int g_currentWeight = 0;   // poids courant du sac

int maxCodom1(int index) {
    int max = g_items[index].value;
    for (int i = index + 1; i < g_n - (index + 1); ++i) {
        if(max < g_items[i].value) {
            max = g_items[i].value;
        }
    }
    return max;
}

int minCodom1(int index) {
    int min = g_items[index].value;
    for (int i = index + 1; i < g_n - (index + 1); ++i) {
        if(min > g_items[i].value) {
            min = g_items[i].value;
        }
    }
    return min;
}

// implémentation récursive de l'algorithme de backtracking
void knapsackBT1(int idx) {
    int absExtT = max(abs(maxCodom1(idx)), abs(minCodom1(idx)));

    if ((absExtT * (g_n - idx) >= abs(g_currentValue))) {
        // Si l'indice actuel est hors le tableau ou si le sac à dos est plein, mettre à jour bestValue si nécessaire
        if (idx == g_n || g_currentWeight == g_W) {
            g_bestValue = max(g_bestValue, g_currentValue);
            return;
        }
        
        // sauter l'objet courant
        knapsackBT1(idx + 1);

        // Inclure l'objet actuel s'il ne dépasse pas la capacité du sac à dos
        if (g_currentWeight + g_items[idx].weight <= g_W) {
            // Mettre à jour currentWeight et currentValue avant l'appel récursif
            g_currentWeight += g_items[idx].weight;
            g_currentValue += g_items[idx].value;

            knapsackBT1(idx + 1);

            // Annuler la décision d'inclure l'objet (backtracking)
            g_currentWeight -= g_items[idx].weight;
            g_currentValue -= g_items[idx].value;
        }
        
    }
}

// Algorithme de backtracking sans variables globales
int knapsackBTUtil(Item items[], int n, int W, int idx, int currentValue, int *bestValue) {
    if (idx == n || W == 0) {
        if (currentValue > *bestValue)
            *bestValue = currentValue;
        return *bestValue;
    }

    // Ne pas inclure l'objet idx
    knapsackBTUtil(items, n, W, idx + 1, currentValue, bestValue);

    // Inclure l'objet idx si le poids le permet
    if (items[idx].weight <= W) {
        knapsackBTUtil(items, n, W - items[idx].weight, idx + 1, currentValue + items[idx].value, bestValue);
    }

    return *bestValue;
}

int knapsackBT2(Item items[], int n, int W) {
    int bestValue = 0;
    return knapsackBTUtil(items, n, W, 0, 0, &bestValue);
}

// Fonction de programmation dynamique pour le problème du sac à dos 0/1.
int knapsackDP(int W, Item items[], int n) {
    int i, w;
    int **K = (int**)malloc((n+1) * sizeof(int*));
    for(i = 0; i <= n; i++)
        K[i] = (int*)malloc((W+1) * sizeof(int));

    for(i = 0; i <= n; i++) {
        for (w = 0; w <= W; w++) {
            if (i == 0 || w == 0)
                K[i][w] = 0;
            else if (items[i-1].weight <= w)
                K[i][w] = max(items[i-1].value + K[i-1][w-items[i-1].weight], K[i-1][w]);
            else
                K[i][w] = K[i-1][w];
        }
    }

    int result = K[n][W];
    for (i = 0; i <= n; i++)
        free(K[i]);
    free(K);
    return result;
}

int knapsackDP_Value(int W, Item items[], int n) {
    int V = 0;
    for (int i = 0; i < n; i++) {
        V += items[i].value;
    }
    
    int dp[V + 1];
    dp[0] = 0;
    for (int v = 1; v <= V; v++) {
        dp[v] = 9999;
    }
    
    for (int i = 0; i < n; i++) {
        for (int v = V; v >= items[i].value; v--) {
            dp[v] = min(dp[v], dp[v - items[i].value] + items[i].weight);
        }
    }
    
    int max_value = 0;
    for (int v = 0; v <= V; v++) {
        if (dp[v] <= W) {
            max_value = v;
        }
    }
    
    return max_value;
}

int knapsackDP_Matrix(int W, int M, Item items[], int n) {
    int **dp = (int**)malloc((n + 1) * sizeof(int*));
    for(int i = 0; i <= n; i++) {
        dp[i] = (int*)malloc((W + 1) * sizeof(int));
    }

    for(int i = 0; i <= n; i++) {
        for (int w = 0; w <= W; w++) {
            if (i == 0 || w == 0)
                dp[i][w] = 0;
            else if (items[i-1].weight <= w)
                dp[i][w] = max(items[i-1].value + dp[i-1][w-items[i-1].weight], dp[i-1][w]);
            else
                dp[i][w] = dp[i-1][w];
        }
    }

    int result = dp[n][W];
    for (int i = 0; i <= n; i++)
        free(dp[i]);
    free(dp);
    return (result >= M) ? result : -1;

}

int partition(Item items[], float arr[], int low, int high) {
    float pivot = arr[high];
    int i = (low - 1); // Index du plus petit élément

    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
            swapitem(&items[i], &items[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    swapitem(&items[i + 1], &items[high]);
    return (i + 1);
}

// Fonction principale qui implémente quicksort
void quickSort(Item items[], float arr[], int low, int high) {
    if (low < high) {
        // pi est l'index de partition, arr[pi] est maintenant à la bonne position
        int pi = partition(items, arr, low, high);
        // Trier les éléments séparément avant et après la partition
        quickSort(items, arr, low, pi - 1);
        quickSort(items, arr, pi + 1, high);
    }
}

int knapsackGlouton(int W, Item items[], int n) {
    float arr[n + 1];
    for (int i = 0; i < n; i++) {
        arr[i] = items[i].value / items[i].weight;
    }
    quickSort(items, arr, 1, n);
    int totalW = W, totalV = 0;

    for(int i = n - 1; i >= 0; i--) {
        if (items[i].weight <= totalW) {
            totalW -= items[i].weight;
            totalV += items[i].value;
        }
    }
    return totalV;
}


int main() {
    FILE *fp = fopen("instances.csv", "r");
    FILE *out = fopen("average_times.csv", "w");
    FILE *sol = fopen("solutions.csv", "w");
    if (!fp || !out || !sol) {
        printf("Erreur lors de l'ouverture des fichiers.\n");
        return 1;
    }

    // Variables pour stocker les somme des temps et compter les instances par taille de problème
    double sumTimeBT1 = 0, sumTimeBT2 = 0, sumTimeDP = 0, sumTimeDP_Value = 0, sumTimeDP_Matrix = 0, sumTimeGL;
    int count = 0, currentN = -1;

    char line[4096];
    while (fgets(line, 4096, fp)) {
        int W, M, n;
        sscanf(line, "%d,%d,%d", &W, &M, &n);

        // Vérifiez si nous passons à une nouvelle taille de problème
        if (n != currentN) {
            if (count > 0) {
                // Écrivez les moyennes pour la taille de problème précédente
                fprintf(out, "%d, %f, %f, %f, %f, %f, %f\n",
                        currentN, 
                        sumTimeBT1 / count, 
                        sumTimeBT2 / count, 
                        sumTimeDP / count, 
                        sumTimeDP_Value / count, 
                        sumTimeDP_Matrix / count,
                        sumTimeGL / count);
            }
            // Réinitialisez les compteurs pour la nouvelle taille
            currentN = n;
            sumTimeBT1 = sumTimeBT2 = sumTimeDP = sumTimeDP_Value = sumTimeDP_Matrix = sumTimeGL = 0;
            count = 0;
        }

        Item items[n];
        char *token = strtok(line, ",");
        token = strtok(NULL, ","); // Passer les deux premiers tokens (W et n)
        token = strtok(NULL, ","); // ligne ajoutee
        for (int i = 0; i < n && token != NULL; i++) {
            sscanf(token, "%d", &items[i].weight);
            token = strtok(NULL, ",");
            sscanf(token, "%d", &items[i].value);
            token = strtok(NULL, ",");
        }

        // Ici, insérez la logique pour configurer et appeler vos fonctions de sac à dos.
        printf("n=%d, W=%d\n", n, W);

        g_items = items;
        g_n = n;
        g_W = W;
        g_bestValue = 0;       // meilleure valeur trouvée jusqu'à présent
        g_currentValue = 0;    // valeur courante du sac
        g_currentWeight = 0;

        clock_t start = clock();
        knapsackBT1(0);
        int maxValBT1 = g_bestValue;
        clock_t end = clock();
        double timeBT1 = (double)(end - start) / CLOCKS_PER_SEC;
        sumTimeBT1 += timeBT1;

        start = clock();
        int maxValBT2 = knapsackBT2(items, n, W);
        end = clock();
        double timeBT2 = (double)(end - start) / CLOCKS_PER_SEC;
        sumTimeBT2 += timeBT2;

        start = clock();
        int maxValDP = knapsackDP(W, items, n);
        end = clock();
        double timeDP = (double)(end - start) / CLOCKS_PER_SEC;
        sumTimeDP += timeDP;

        start = clock();
        int maxValDP_Value = knapsackDP_Value(W, items, n);
        end = clock();
        double timeDP_Value = (double)(end - start) / CLOCKS_PER_SEC;
        sumTimeDP_Value += timeDP_Value;

        start = clock();
        int maxValDP_Matrix = knapsackDP_Matrix(W, M, items, n);
        end = clock();
        double timeDP_Matrix = (double)(end - start) / CLOCKS_PER_SEC;
        sumTimeDP_Matrix += timeDP_Matrix;

        start = clock();
        int maxValGL = knapsackGlouton(W, items, n);
        end = clock();
        double timeGL = (double)(end - start) / CLOCKS_PER_SEC;
        sumTimeGL += timeGL;

        count++;

        fprintf(sol, "%d, %d, %f, %d, %f, %d, %f, %d, %f, %d, %f, %d, %f, %d\n",
            n, W, 
            timeBT1, maxValBT1, 
            timeBT2, maxValBT2, 
            timeDP, maxValDP, 
            timeDP_Value, maxValDP_Value, 
            timeDP_Matrix, maxValDP_Matrix,
            timeGL, maxValGL);
    }

    // N'oubliez pas d'écrire les moyennes pour la dernière taille de problème
    if (count > 0) {
        fprintf(out, "%d, %f, %f, %f, %f, %f\n",
                currentN, 
                sumTimeBT1 / count, 
                sumTimeBT2 / count, 
                sumTimeDP / count, 
                sumTimeDP_Value / count,
                sumTimeDP_Matrix / count,
                sumTimeGL / count);
    }

    fclose(fp);
    fclose(out);
    fclose(sol);
    return 0;
}
