import pandas as pd
import matplotlib.pyplot as plt

# Chemin vers le fichier CSV contenant les temps d'exécution moyens
csv_file_path = 'average_times.csv'

# Lire le fichier CSV dans un DataFrame pandas
# On suppose que le fichier a 4 colonnes : Taille du problème, Temps BT1, Temps BT2, Temps DP
df = pd.read_csv(csv_file_path, header=None, names=['Taille', 'BT1', 'BT2', 'DP'])

# Préparation du graphique pour comparer les temps d'exécution des trois algorithmes
plt.figure(figsize=(10, 6))
plt.plot(df['Taille'], df['BT1'], label='Backtracking 1', marker='o')
plt.plot(df['Taille'], df['BT2'], label='Backtracking 2', marker='s')
plt.plot(df['Taille'], df['DP'], label='Programmation Dynamique', marker='^')

# Ajout de titres et légendes pour le graphique
plt.title('Comparaison des temps d\'exécution des algorithmes pour le problème du sac à dos')
plt.xlabel('Taille du problème (nombre d\'objets)')
plt.ylabel('Temps d\'exécution moyen (secondes)')
plt.legend()

# Affichage de la grille
plt.grid(True)

# Sauvegarder le graphique sur le disque
plt.savefig('comparaison_temps_execution.png')

# Afficher le graphique
plt.show()

# Quitter proprement
plt.close()
