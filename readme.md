# Execution et affichage d'[IFS](https://en.wikipedia.org/wiki/Iterated_function_system) sur GPU

## Présentation
Ce petit projet a pour but d'explorer une façon de visualiser des IFS sur GPU, en évitant de stocker le résultat,
À l'aide d'un codage arithmétique de chacune des branches de l'arbre formé par l'IFS.
Pour plus de détails, vous pouvez lire mon petit rapport `rapport.pdf`.

## Executables
Dans l'onglet *Release* vous avez un executable pour Windows, et une pour Linux générant le triangle de Sierpiński.

## Commandes
- Flèches directionnelles:
    - Droite/gauche: rotation de la caméra autour de 0, 0, 0.
    - Devant/derrière: rapprocher, reculer la caméra.
- '+': Augmenter le nombre d'itération
- '-': diminuer le nombre d'itération
- 'r': Activer/Désactiver la [rasterization conservative](https://developer.nvidia.com/gpugems/gpugems2/part-v-image-oriented-computing/chapter-42-conservative-rasterization),
- 's': alterner entre génération de l'IFS sur CPU ou GPU

# Compiler le projet
## Clônage avec dépendances:
`git clone --recursive https://github.com/Gibus21250/Fractal-IFS.git`

## Compilation
Prérequis:
- [cmake](https://cmake.org/)
- [Vulkan-SDK](https://vulkan.lunarg.com/)
- C11 et C++11 compilateur

Placer vous dans le repertoire tout juste clôné, puis executé la commande:

`cmake -B"build"`

Vous pouvez spécifier votre générateur de projet comme cela:

`cmake -B"build" -G"generator"`

Remplacez "generator" par votre générateur, par exemple sur linux cela peut être `Unix Makefiles`, sur windows `Ninja` ou `Visual Studio 17 2022`.

Une fois cela fait, placez-vous dans le dossier build, et executé la commande de votre générateur.

![Flocon](https://github.com/Gibus21250/Fractal-IFS/blob/master/Flocon%20de%20Pythagore.png?raw=true)
