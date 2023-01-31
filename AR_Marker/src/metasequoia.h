/* ****************************************************** metasequoia.h *** *
 * 3D Model - Metasequoia Class
 * ************************************************************************* */
#pragma once

#include "GLMetaseq.h"

class Metasequoia {
    public:
        // Constructor
        Metasequoia();
        Metasequoia(char* filename);

        // Destructor
        ~Metasequoia ();

        // Open 3D Model
        void OpenModel (char* filename);
        // Draw 3D Model
        void DrawModel (void);
  
        // 3D Model
        MQO_MODEL model;
        double    scale;

};

/* ************************************************ End of metasequoia.h *** */
