import cv2
import cv2.aruco as aruco
import numpy as np
import time

# Charger dict ArUco
aruco_dict = aruco.getPredefinedDictionary(aruco.DICT_4X4_50)

# Taille marqueur en m (2,25 cm)
marker_real_size = 0.0225

# Matrice calib ajustée
camera_matrix = np.array([[485, 0, 640], [0, 485, 360], [0, 0, 1]], dtype=np.float32)
dist_coeffs = np.zeros((5, 1))  # Pas distorsion

print("Matrice calib ajustée :\n", camera_matrix)
print("Coeffs distorsion :\n", dist_coeffs)

# Calib distance
displayed_distance = 0.33  # Affichée en m
real_distance = 0.20       # Réelle en m
distance_scale = real_distance / displayed_distance  # Facteur échelle
print(f"Facteur d'échelle : {distance_scale:.2f}")

# Setup cam
cap = cv2.VideoCapture(0)
if not cap.isOpened():
    print("Erreur cam.")
    exit()

print("Appuyez sur 'q' pour quitter.")

# Timer pour print 0.1s
last_print_time = time.time()

while True:
    ret, frame = cap.read()
    if not ret:
        print("Erreur lecture flux.")
        break

    # Taille image
    frame_height, frame_width = frame.shape[:2]
    center_x, center_y = frame_width // 2, frame_height // 2

    # Gris
    gray_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    # Viseur croix
    cv2.line(frame, (center_x, 0), (center_x, frame_height), (0, 255, 0), 1)  # Vert V
    cv2.line(frame, (0, center_y), (frame_width, center_y), (0, 255, 0), 1)   # Vert H

    # Detecter marqueurs
    corners, ids, rejected = aruco.detectMarkers(gray_frame, aruco_dict)

    # Si des ids trouvés
    if ids is not None:
        aruco.drawDetectedMarkers(frame, corners, ids)

        # Estimer pose pour chaque marqueur
        rvecs, tvecs, _ = aruco.estimatePoseSingleMarkers(corners, marker_real_size, camera_matrix, dist_coeffs)
        for rvec, tvec, corner, marker_id in zip(rvecs, tvecs, corners, ids.flatten()):
            # Calcul distance en m avec échelle
            raw_distance = np.linalg.norm(tvec)  # Brute
            distance = raw_distance * distance_scale  # Ajustée

            # Centre marqueur
            marker_center_x = int((corner[0][0][0] + corner[0][2][0]) / 2)
            marker_center_y = int((corner[0][0][1] + corner[0][2][1]) / 2)

            # Calcul angle
            x_offset_pixels = marker_center_x - center_x  # Décalage X
            angle = np.degrees(np.arctan2(x_offset_pixels, frame_width))  # Angle en deg

            # Direction
            direction = "Droit" if angle == 0 else ("Droite" if angle > 0 else "Gauche")

            # Print toutes les 0.1s
            current_time = time.time()
            if current_time - last_print_time >= 0.1:
                print(f"ID: {marker_id}, Dist: {distance:.2f} m, Angle: {angle:.2f}° ({direction})")
                last_print_time = current_time

            # Annoter image
            cv2.putText(frame, f"Dist: {distance:.2f}m",
                        (center_x - 100, 30), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 255, 255), 2)
            cv2.putText(frame, f"Angle: {angle:.2f}° ({direction})",
                        (center_x - 100, 60), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 255, 255), 2)

            # Marquer centre
            cv2.circle(frame, (marker_center_x, marker_center_y), 5, (0, 0, 255), -1)

            # Dessiner axe 3D
            cv2.drawFrameAxes(frame, camera_matrix, dist_coeffs, rvec, tvec, 0.05)

    # Show cam
    cv2.imshow("Guidage Robot avec Viseur ArUco", frame)

    # Quitter 'q'
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# Libérer
cap.release()
cv2.destroyAllWindows()
