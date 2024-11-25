# Traitement d'image avec ArUco pour Guidage Robot

Script qui utilise des marqueurs ArUco et une caméra pour guider le robot en fournissant des informations sur la distance et l'orientation par rapport aux marqueurs visibles. Il capte un flux vidéo, détecte les marqueurs ArUco, et calcule la distance ainsi que l'angle en temps réel.


## Prérequis
- **OpenCV avec modules contrib** (pour `cv2.aruco`)

### Installation des bibliothèques requises
Installe OpenCV avec les modules contrib via `pip` :
```bash
pip install opencv-contrib-python
```

## Comment utiliser le script

### 1. Préparer le Matériel
- **Test materiel** : Vérifier le flux video sur la camera et la résolution 


### 2. Lancer le Script
- Exécute le script avec la commande suivante :
  ```bash
  python processing_arUco.py
  ```
- Oriente la caméra vers un marqueur ArUco visible.

### 3. Interagir avec le Flux Vidéo
- Une fois le script lancé, tu verras le flux vidéo avec un **viseur vert en croix** et des informations superposées.

## Explications du Fonctionnement

### Détection des Marqueurs ArUco
- Le script charge le dictionnaire de marqueurs ArUco (`DICT_4X4_50`) et détecte les marqueurs dans chaque image du flux vidéo.
- Une fois les marqueurs détectés, le script calcule leur pose avec `cv2.aruco.estimatePoseSingleMarkers`.

### Calcul de la Distance et de l'Angle
- La **distance** est calculée à partir du vecteur de translation du marqueur par rapport à la caméra, ajustée avec un facteur d'échelle (`distance_scale`).
- L'**angle** est calculé en comparant le centre du marqueur avec le centre de l'image pour déterminer si le robot doit se déplacer vers la **gauche**, la **droite**, ou continuer **tout droit**.

### Affichage des Informations
- Le script **annote** l'image avec :
  - **La distance** en mètres
  - **L'angle** en degrés, avec la direction
  - Une **croix verte** pour voir si le marqueur est aligné.

## Ajustements Personnalisés

- **Taille du Marqueur** :
  - Modifie `marker_real_size` si tu utilises une taille différente :
    ```python
    marker_real_size = 0.03  # Taille en mètres (ex : 3 cm)
    ```

- **Correction de la Distance** :
  - Ajuste le facteur d'échelle (`distance_scale`) si nécessaire :
    ```python
    distance_scale = real_distance / displayed_distance
    ```
    - `real_distance` : Distance mesurée dans la réalité.
    - `displayed_distance` : Distance affichée par le script.



