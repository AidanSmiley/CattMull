#include "Curve.h"
#include <iomanip>

Curve::Curve()
{
}

Curve::~Curve()
{
}

void Curve::init()
{
	this->control_points_pos = {
		{ 0.0, 8.5, -2.0 },
		{ -3.0, 11.0, 2.3 },
		{ -6.0, 8.5, -2.5 },
		{ -4.0, 5.5, 2.8 },
		{ 1.0, 2.0, -4.0 },
		{ 4.0, 2.0, 3.0 },
		{ 7.0, 8.0, -2.0 },
		{ 3.0, 10.0, 3.7 }
	};
}

void Curve::calculate_curve()
{
	//i think i implement my catmull-rom here?
	
	/*
	this->curve_points_pos = {
		{ 0.0, 8.5, -2.0 },		//p0
		{ -3.0, 11.0, 2.3 },	//p1
		{ -6.0, 8.5, -2.5 },	//p2
		{ -4.0, 5.5, 2.8 },		//p3
		{ 1.0, 2.0, -4.0 },		//p4
		{ 4.0, 2.0, 3.0 },		//p5
		{ 7.0, 8.0, -2.0 },		//p6
		{ 3.0, 10.0, 3.7 }		//p7
	};
	*/

	std::vector<glm::vec4> coef = {
		{-0.5,  1.5, -1.5,  0.5},
		{ 1.0, -2.5,  2.0, -0.5},
		{-0.5,  0.0,  0.5,  0.0},
		{ 0.0,  1.0,  0.0,  0.0}
	};
	std::vector<glm::vec3> points;
	std::vector<glm::vec3> calcPoints;

	//for loop to get through and make all point matrices
	for (int i = 1; i <= 8; i++) {
		//std::cout << "pMat" << i << " is " << (i - 1) % 8 << " " << i % 8 << " " << (i + 1) % 8 << " " << (i + 2) % 8 << std::endl;
		
		/*
			creates point matrix to be sent into the point calculating function
			p i-1
			p i
			p i+1
			p i+2
		*/
		points.push_back(control_points_pos[(i - 1) % 8]); // i-1
		points.push_back(control_points_pos[(i    ) % 8]); // i
		points.push_back(control_points_pos[(i + 1) % 8]); // i+1
		points.push_back(control_points_pos[(i + 2) % 8]); // i+2

		//calculate 200 points for the segment between i and i+1
		calcPoints = calculate_segment(coef, points);

		
		for (int i = 0; i < calcPoints.size(); i++) {
			this->curve_points_pos.push_back(calcPoints[i]);
		}

		points.clear();
	}

	
}


std::vector<glm::vec3> Curve::calculate_segment(std::vector<glm::vec4> coef, std::vector<glm::vec3> points) {

	std::vector<glm::vec3> tempMat;
	std::vector<glm::vec3> newPoints;

	glm::vec4 columnVec;
	glm::vec4 uVec = glm::vec4(0, 0, 0, 1);

	//this is basically matrix multiplication for a 4x4 times a 3x4
	//the matrix will be in tempMat
	for (int i = 0; i < 4; i++) {
		glm::vec3 temp = glm::vec3(0.0, 0.0, 0.0);

		//x
		columnVec.x = points[0].x;
		columnVec.y = points[1].x;
		columnVec.z = points[2].x;
		columnVec.w = points[3].x;


		temp.x = glm::dot(coef[i], columnVec);


		//y
		columnVec.x = points[0].y;
		columnVec.y = points[1].y;
		columnVec.z = points[2].y;
		columnVec.w = points[3].y;


		temp.y = glm::dot(coef[i], columnVec);

		//z
		columnVec.x = points[0].z;
		columnVec.y = points[1].z;
		columnVec.z = points[2].z;
		columnVec.w = points[3].z;


		temp.z = glm::dot(coef[i], columnVec);

		tempMat.push_back(temp);
	}

	//take that matrix and mutliply is by the u matrix 200 times
	//each iteration results in a new point that is put into the newPoints vector
	for (int j = 1; j < 200; j++) {
		glm::vec3 temp = glm::vec3(0.0, 0.0, 0.0);
		float frac = j / 200.0;

		//construct u
		uVec.x = pow(frac, 3.0);
		uVec.y = pow(frac, 2.0);
		uVec.z = frac;

		//printf("x: %f y: %f z: %f\n", uVec.x, uVec.y, uVec.z);

		columnVec.x = tempMat[0].x;
		columnVec.y = tempMat[1].x;
		columnVec.z = tempMat[2].x;
		columnVec.w = tempMat[3].x;

		temp.x = glm::dot(uVec, columnVec);

		columnVec.x = tempMat[0].y;
		columnVec.y = tempMat[1].y;
		columnVec.z = tempMat[2].y;
		columnVec.w = tempMat[3].y;

		temp.y = glm::dot(uVec, columnVec);

		columnVec.x = tempMat[0].z;
		columnVec.y = tempMat[1].z;
		columnVec.z = tempMat[2].z;
		columnVec.w = tempMat[3].z;

		temp.z = glm::dot(uVec, columnVec);

		//printf("x: %f y: %f z: %f\n", temp.x, temp.y, temp.z);

		newPoints.push_back(temp);

	}

	return newPoints;
}



//it dont work :(

/*
std::vector<glm::vec3> Curve::calculate_segment(std::vector<glm::vec4> coef, std::vector<glm::vec3> points) {
	glm::mat4 pointMat;
	glm::mat4 coefMat;
	glm::mat4 pointXcoef;

	glm::vec4 uVec = glm::vec4(0.0, 0.0, 0.0, 1.0);
	
	std::vector<glm::vec3> newPoints;

	//constuct a 4x4 matrix of the points
	for (int i = 0; i < points.size(); i++) {
		//need to add extra zero to make it 4x4
		pointMat[i] = glm::vec4(points[i].x, points[i].y, points[i].z, 0.0);
	}

	//construct a 4x4 matrix of the coefficient
	for (int i = 0; i < coef.size(); i++) {
		coefMat[i] = coef[i];
	}

	pointXcoef = coefMat * pointMat; //i hope this multiplied right . . .

	//multiply the pointXcoef matrix by u 200 times
	for (int i = 1; i < 200; i++) {
		glm::vec4 temp = glm::vec4(0.0, 0.0, 0.0, 0.0);
		float frac = i / 200.0;

		//construct u
		uVec.x = pow(frac, 3.0);
		uVec.y = pow(frac, 2.0);
		uVec.z = frac;

		// u * pointXcoef
		temp = uVec * pointXcoef;

		newPoints.push_back(glm::vec3(temp.x, temp.y, temp.z));
	}

	return newPoints;
}
*/