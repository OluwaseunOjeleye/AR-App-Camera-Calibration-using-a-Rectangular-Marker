/* ****************************************************** metasequoia.c *** *
 * 3D Model - Metasequoia
 * ************************************************************************* */
#include "metasequoia.h"

/*
 * Constructor
 */
Metasequoia::Metasequoia() {
  scale = 1.0;
}

/*
 * Costructor
 */
Metasequoia::Metasequoia(char* filename) {
  scale = 1.0;  
  model = mqoCreateModel (filename, scale);
}

/*
 * Destructor
 */
Metasequoia::~Metasequoia() {
  ;
}

/*
 * Open 3D Model
 *
 * @param [in] filename
 */
void Metasequoia::OpenModel (char* filename) {
  model = mqoCreateModel (filename, scale);
}

/*
 * Draw 3D Model
 */
void Metasequoia::DrawModel (void) {
  mqoCallModel (model);
}

/* ************************************************ End of metasequoia.c *** */
