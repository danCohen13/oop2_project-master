#pragma once

// Regroupe le chargement de toutes les ressources (textures, sons, police)
// au démarrage de l'application. Permet à Controller de ne plus connaître
// les noms de fichiers ni les clés de ressources.
namespace AssetLoader {
    void loadAll();
}