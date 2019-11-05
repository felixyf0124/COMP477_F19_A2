

#include "objloader.hpp"

#pragma warning(disable:4996)

bool loadOBJ(
	const char * path,
	std::vector<GLfloat> & out_vertices,
	std::vector<GLuint> & out_indices){

	

	FILE * file = fopen(path, "r");
	if (file == NULL){
		printf("Impossible to open the file ! Are you in the right path ?\n");
		getchar();
		return false;
	}

	while (1){

		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

		// else : parse lineHeader

		if (strcmp(lineHeader, "v") == 0){
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			out_vertices.push_back(vertex.x);
			out_vertices.push_back(vertex.y);
			out_vertices.push_back(vertex.z);
		}
		
		else if (strcmp(lineHeader, "f") == 0){
			
			GLuint indices[3];
			int matches = fscanf(file, "%i %i %i\n", &indices[0], &indices[1], &indices[2]);
			if (matches != 3){
				printf(matches+"\n");
				printf("File can't be read by our simple parser :-( Try exporting with other options\n");
				return false;
			}
			out_indices.push_back(indices[0]);
			out_indices.push_back(indices[1]);
			out_indices.push_back(indices[2]);
			
			
		}
		else{
			// Probably a comment, eat up the rest of the line
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}

	}

	//since the exported .obj file with the indices start at 1 
	//so for all indices -1
	for (GLuint i = 0; i < out_indices.size(); ++i) {
		--out_indices[i];
	}

	return true;
}
