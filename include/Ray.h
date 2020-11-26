#pragma once

namespace raycast
{
    class ray
    {
        public:

        glm::vec3 start;
        glm::vec3 end;
        glm::vec3 direction;
        float distance;
    };

    class result
    {
        public:
        bool hit;
    };

    result CastRay(ray inRay, model_t& target, transform& inTrans)
    {
        //uhh go through every triangle loaded to determine what is hit. i don't have collision volumes...so triangles

        //in the future just go through all models loaded in the player view frustum
        bool hit = false;
        for (auto meshIter : target.meshes)
        {
            for (unsigned int vertexIter = 2; vertexIter < meshIter.vertices.size(); vertexIter++)
            {
                //ok make the triangle  and apply the raycast math
                
                glm::mat4 position = glm::translate(glm::mat4(1.0f), glm::vec3(inTrans.position));;
                glm::mat4 rotation = glm::toMat4(inTrans.rotation);
                glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(inTrans.scale));

                //transform the position via PRS transforms
                glm::mat4 PRS = position * rotation * scale;
                glm::vec3 P0 = meshIter.vertices[vertexIter - 2].position;
                glm::vec3 P1 = meshIter.vertices[vertexIter - 1].position;
                glm::vec3 P2 = meshIter.vertices[vertexIter].position;

                glm::vec3 N = glm::cross(P1 - P0, P2 - P0);
                float d = glm::dot(-N, P0);
                glm::vec4 L = glm::vec4(N.x, N.y, N.z, d);

                float intersectPoint1 = glm::dot(L, glm::vec4(inRay.start, 1));
                float intersectPoint2 = glm::dot(L, glm::vec4(inRay.direction, 1));
                

                float t = -(intersectPoint1 / intersectPoint2);

                glm::vec3 point = inRay.start + (t * inRay.direction);

                //printf("%f | %f | %f | %f | %f \n", intersectPoint2, t, point.x, point.y, point.z);
                

                //ok now what to look for?
                
                if(t > 0) //how the fuck do i tell if there has been a collision?
                {
                    hit = true;
                }

				//ray VS sphere test 1
				/*float radius = pow(inRay.start.x + (t * inRay.direction.x), 2) + pow(inRay.start.y + (t * inRay.direction.y), 2) + pow(inRay.start.z + (t * inRay.direction.z), 2);
				float a = glm::length(glm::vec3(inRay.direction * inRay.direction));
				float b = dot(inRay.start, inRay.direction) * 2;
				float c = pow(glm::length(inRay.start), 2.0f) - pow(radius, 2.0f);
				float D = pow(b, 2) - (4 * (a * c));
                printf("%f \n", D);*/
                

                //ray vs sphere test 2
                glm::vec3 p = glm::vec3(meshIter.vertices[vertexIter].position);
                float t2 = -1.0f * glm::dot(inRay.start - p, inRay.direction);
                if(t2 < 0.0f)
                {
                    t2 = 0.0f;
                }

                auto closest = inRay.start + t * inRay.direction;
                float dist2 = glm::dot(p - closest, p - closest);
                float radius = 1;
                
                if (dist2 < radius)
                {
                    printf("%f \n", dist2);
                    printf("hit! \n");
                }
            }
        }
		result res;
		res.hit = hit;
        return res;
    }

	result RayFromMouse(camera& inCamera, glm::vec2 mousePosition, model_t& target, transform& inTrans)
	{
        //should ray start be camera position? camera pos, to infinity?
        //from camera to model?
        //auto rayStart = glm::vec4(inCamera.position, 1);
        //auto rayEnd = glm::vec4(target.position, 1);

        auto invMat = glm::inverse(inCamera.projection * inCamera.view);

        auto rayStart = inCamera.view[3];// //invMat * glm::vec4(mousePosition.x, mousePosition.y, 0.0f, 1.0f);
        auto rayEnd = inCamera.view[2];// *glm::vec4(mousePosition.x, mousePosition.y, 1.0f - glm::epsilon<float>(), 1.0f);
        //i think ray end is mean to be a discrete value
		//scaling?
		//rayStart /= rayStart.w;
		//rayEnd /= rayEnd.w;

		auto rayVel = glm::normalize(rayEnd - rayStart);

		ray newRay;
		newRay.start = rayStart;
		newRay.end = rayEnd;
		newRay.direction = rayVel;

		return CastRay(newRay, target, inTrans);
	}
}



