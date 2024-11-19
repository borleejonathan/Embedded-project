# Guide de collaboration Git pour le Projet Embedded

Bienvenue la Team Freddy! Voici un document pour avoir les bases de l'utilisation de Git et participer efficacement au projet Eurobot2025

le github est disponible à l'adresse suivante : (vous êtes probablement déjà dessus)

[https://github.com/DiegoRadigues/Embedded-project.git](https://github.com/DiegoRadigues/Embedded-project.git)



Vous allez apprendre à forker, cloner, créer des branches, effectuer des commits, faire des pull requests, et plus encore.

## 1. Forker le dépôt

La première étape pour travailler sur ce projet est de forker le dépôt principal :

1. Allez sur la page GitHub du projet en utilisant le lien ci-dessus.
2. Cliquez sur le bouton **Fork** en haut à droite pour créer une copie du projet sur votre propre compte GitHub. Cela vous permet d'avoir votre propre version du projet, sur laquelle vous pourrez travailler sans affecter le dépôt principal.

Un fork est une copie personnelle du projet hébergée sur votre propre compte GitHub. Toutes les modifications que vous faites dans votre fork ne sont pas appliquées directement au projet principal, vous devez d'abord proposer ces modifications via une Pull Request (voir étape 8).

## 2. Cloner le dépôt

Après avoir forké le dépôt, vous devez cloner votre fork sur votre machine locale pour travailler dessus :

```bash
$ git clone https://github.com/votre-utilisateur/Embedded-project.git
```

- Remplacez `votre-utilisateur` par votre nom d'utilisateur GitHub. Cela créera une copie locale du projet sur votre ordinateur. Assurez-vous d'être dans le dossier où vous souhaitez stocker le projet avant d'exécuter cette commande.

Cette copie locale est celle sur laquelle vous allez travailler. Toutes les modifications seront d'abord faites localement avant d'être poussées vers votre fork sur GitHub.

## 3. Configurer le dépôt principal comme remote

Pour rester synchronisé avec le projet principal, vous devez ajouter le dépôt principal comme un remote appelé `upstream`. Cela vous permettra de récupérer les mises à jour du projet principal :

```bash
$ git remote add upstream https://github.com/DiegoRadigues/Embedded-project.git
```

- `upstream` est le nom que nous donnons au dépôt principal. Cela vous permettra de récupérer les dernières modifications du projet principal.
- Pour vérifier que le remote a bien été ajouté, exécutez la commande suivante :

  ```bash
  $ git remote -v
  ```
  Vous devriez voir `origin` pointant vers votre fork et `upstream` pointant vers le dépôt principal.

## 4. Créer une branche pour chaque fonctionnalité

Pour éviter les conflits, il est essentiel de travailler sur des branches séparées pour chaque nouvelle fonctionnalité ou correction de bug. Cela vous permet de garder la branche `main` propre et d'isoler les développements spécifiques :

1. Naviguez vers le dossier du projet cloné :

   ```bash
   $ cd Embedded-project
   ```

2. Créez une nouvelle branche :

   ```bash
   $ git checkout -b nom-de-la-branche
   ```

   - Remplacez `nom-de-la-branche` par un nom descriptif qui indique ce sur quoi vous travaillez, par exemple `ajout-formulaire-utilisateur` ou `correction-bug-auth`.
   - Le nom de la branche doit être clair et concis afin que les autres membres de l'équipe puissent comprendre ce qui est fait dans cette branche.

Travailler sur une branche séparée évite les conflits de code entre les membres de l'équipe et permet une meilleure gestion des versions.

## 5. Faire des commits régulièrement

Lors de vos modifications, faites des commits souvent et fournissez des messages clairs et explicites sur ce que vous avez fait. Cela facilitera la compréhension de vos modifications par les autres membres de l'équipe et vous aidera à suivre vos propres changements :

1. **Ajouter les fichiers modifiés au commit** :

   ```bash
   $ git add fichier-modifie.txt
   ```

   - Vous pouvez également ajouter tous les fichiers modifiés avec :

     ```bash
     $ git add .
     ```

2. **Faire le commit** :

   ```bash
   $ git commit -m "Ajout du formulaire de connexion utilisateur"
   ```

   - Assurez-vous que chaque commit soit spécifique et décrive clairement la modification apportée. Évitez des messages comme "modifications diverses".
   - Un bon message de commit devrait répondre à la question "qu'est-ce que ce commit change ?".

3. **Diviser les commits** :
   - Essayez de faire des commits logiques et bien séparés. Par exemple, si vous ajoutez une fonctionnalité et corrigez un bug, faites deux commits séparés.

## 6. Récupérer les changements depuis le dépôt principal

Avant de pousser vos modifications ou de soumettre une pull request, il est important de synchroniser votre branche avec le dépôt principal pour éviter les conflits potentiels :

1. **Récupérer les dernières modifications** depuis le dépôt principal (`upstream`) :

   ```bash
   $ git fetch upstream
   ```

2. **Aller sur la branche principale locale** :

   ```bash
   $ git checkout main
   ```

3. **Fusionner les modifications récupérées** :

   ```bash
   $ git merge upstream/main
   ```

   - Si des conflits apparaissent, vous devrez les résoudre avant de continuer. Cela signifie comparer les différences et choisir quelles modifications conserver.

Récupérer et fusionner régulièrement les changements de `upstream/main` permet de garder votre fork à jour et d'éviter des surprises lors de la création d'une pull request.

## 7. Pousser les modifications sur votre fork

Une fois que vous avez terminé les modifications et résolu les conflits éventuels, poussez votre branche vers votre fork sur GitHub :

```bash
$ git push origin nom-de-la-branche
```

- Cela enverra les modifications de votre branche locale vers votre dépôt GitHub.
- Assurez-vous de bien pousser sur une branche différente de `main` pour éviter toute confusion.

## 8. Faire une Pull Request (PR)

Après avoir poussé vos modifications sur votre fork, vous devez créer une Pull Request pour proposer vos modifications au dépôt principal :

1. **Allez sur la page de votre fork sur GitHub**.
2. **Cliquez sur le bouton "Compare & pull request"** qui apparaîtra lorsque vous avez une branche récente avec des modifications.
3. **Fournir une description détaillée** :
   - Expliquez ce que vous avez fait, pourquoi vous l'avez fait et ce que cela apporte au projet.
   - Utilisez un titre clair pour la pull request, par exemple : `Ajout de la fonctionnalité de gestion des utilisateurs`.
4. **Assignation et revue** :
   - Assignez la pull request à un autre membre de l'équipe pour la revue.
   - Les autres membres de l'équipe pourront commenter vos modifications, suggérer des améliorations, ou même demander des modifications supplémentaires avant d'approuver et de fusionner la pull request.

## 9. Bonnes pratiques pour éviter les erreurs

- **Travaillez toujours sur une nouvelle branche** : Ne faites jamais de modifications directement sur `main`. La branche `main` doit toujours être stable et refléter l'état actuel de la version de production.
- **Synchronisez souvent** : Avant de commencer à travailler et avant de faire une pull request, synchronisez votre dépôt local avec le dépôt principal (`upstream`). Cela réduit les risques de conflits.
- **Messages de commit clairs** : Chaque message de commit doit décrire précisément la modification. Utilisez l'impératif (par exemple, "Ajoute le support pour la localisation").
- **Petites Pull Requests** : Essayez de limiter la taille de chaque pull request. Une PR plus petite est plus facile à revoir et à tester.
- **Testez avant de faire une PR** : Assurez-vous que votre code fonctionne correctement et que tous les tests passent avant de faire une pull request.

## 10. Commandes Git utiles

- **Voir l'état des fichiers modifiés** :

  ```bash
  $ git status
  ```
  - Cette commande vous donne un aperçu des fichiers modifiés, ajoutés, ou supprimés dans votre projet.

- **Voir les branches disponibles** :

  ```bash
  $ git branch
  ```
  - Utilisez `-a` pour voir toutes les branches, locales et distantes.

- **Changer de branche** :

  ```bash
  $ git checkout nom-de-la-branche
  ```
  - Vous pouvez également créer et basculer directement sur une nouvelle branche avec `-b`.

- **Fusionner une branche** (par exemple `upstream/main` dans `main`) :

  ```bash
  $ git merge upstream/main
  ```
  - En cas de conflits, ouvrez les fichiers mentionnés, résolvez les différences, puis terminez le merge avec :
    
    ```bash
    $ git add fichier-resolu.txt
    $ git commit -m "Résolution des conflits"
    ```

- **Annuler un commit local** : Si vous avez fait une erreur et que vous souhaitez annuler un commit local non poussé :

  ```bash
  $ git reset --soft HEAD~1
  ```
  - Cela vous permet de revenir au commit précédent tout en gardant vos modifications dans l'index.



Je pense que c'est tout, n'hésitez pas à ajouter des modifs à ce guide si vous les jugez pertinentes.

Bon travail à tous !



