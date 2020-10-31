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
        //uhh go through every triangle loaded to determine what is hit

        //in the future just go through all models loaded in the player view frustum
        bool hit = false;
        for (auto meshIter : target.meshes)
        {
            for (unsigned int vertexIter = 2; vertexIter < meshIter.vertices.size(); vertexIter += 3)
            {
                //ok make the triangle  and apply the raycast math
                
                glm::mat4 position = glm::translate(glm::mat4(1.0f), glm::vec3(inTrans.position));;
                glm::mat4 rotation = glm::toMat4(inTrans.rotation);
                glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(inTrans.scale));

                //transform the position via PRS transforms
                glm::mat4 PRS = position * rotation * scale;
                glm::vec3 P0 = PRS * meshIter.vertices[vertexIter - 2].position;
                glm::vec3 P1 = PRS * meshIter.vertices[vertexIter - 1].position;
                glm::vec3 P2 = PRS * meshIter.vertices[vertexIter].position;

                glm::vec3 N = glm::cross(P1 - P0, P2 - P0);
                float d = glm::dot(-N, P0);
                glm::vec4 L = glm::vec4(N.x, N.y, N.z, d);

                float intersectPoint1 = glm::dot(L, glm::vec4(inRay.start, 1));
                float intersectPoint2 = glm::dot(L, glm::vec4(inRay.direction, 1));
                

                float t = -(intersectPoint1 / intersectPoint2);

                glm::vec3 point = inRay.start + (t * inRay.direction);

                printf("%f | %f | %f | %f | %f \n", intersectPoint2, t, point.x, point.y, point.z);
                //why does it give constant false positives?
                //t should be -1 if there is no intersection?
                
                if( t > 0 && t < 1) //how the fuck do i tell if there has been a collision?
                {
                    hit = true;
                    
                    
                   // return res;
                }

                

                //i could going from here to figure out WHERE the ray intersects but i'm tired
            }
        }
		result res;
		res.hit = hit;
        return res;
    }

	result RayFromMouse(camera& inCamera, glm::vec2 mousePosition, model_t& target, transform& inTrans)
	{
		auto rayStart = glm::inverse(inCamera.view * inCamera.projection) * glm::vec4(mousePosition.x, mousePosition.y, 0.0f, 1.0f);
		auto rayEnd = glm::inverse(inCamera.view * inCamera.projection) * glm::vec4(mousePosition.x, mousePosition.y, 1.0f - glm::epsilon<float>(), 1.0f);

		//scaling?
		rayStart /= rayStart.w;
		rayEnd /= rayEnd.w;

		auto rayVel = glm::normalize(rayEnd - rayStart);

		ray newRay;
		newRay.start = rayStart;
		newRay.end = rayEnd;
		newRay.direction = rayVel;

		return CastRay(newRay, target, inTrans);
	}
}



