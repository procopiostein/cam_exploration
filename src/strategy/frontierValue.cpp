#include <cam_exploration/frontierValue.h>
#include <cam_exploration/RobotMotion.h>
#include <cam_exploration/AStar.h>


/** 
 * @file frontierValue.cpp
 * @brief Implementation of frontierValue.h file
 * @author Jordi Soler
 * @version 1.0
 * @date 2016-04-21
 */


#include <iostream>
#include <sstream>

namespace cam_exploration{
namespace strategy{

using namespace std;

typedef map<string, string> mapss;

const char* frontierValue::printInfo(const frontier&) const
{
    return "-- No values specified --";
}



//--------------------------------------------------------------------------------------
//
// 	MAXIMUM SIZE
//
//--------------------------------------------------------------------------------------

maxSize::maxSize(map<string, string> params)
{
    for(map<string, string>::iterator it = params.begin(); it != params.end(); ++it){
    	string name = it->first;
    	string value = it->second;
    	if(name == "weight"){
    	    weight = std::atof(value.c_str());
	}
	else{
	    ROS_WARN("maxSize: String %s does not name a valid parameter", name.c_str());
	}
    }
}

const char* maxSize::name() const { return "maxSize";}

double maxSize::value(const frontier& f) const { return f.size()*weight;}

const char* maxSize::printInfo(const frontier& f) const
{
    std::ostringstream oss;

    oss << "Size: " << f.size();

    return oss.str().c_str();
}


//--------------------------------------------------------------------------------------
//
// 	MINUMUM DISTANCE
//
//--------------------------------------------------------------------------------------

minEuclidianDistance::minEuclidianDistance(map<string, string> params)
{
    weight = 1;
    dispersion = 1;
    for(map<string, string>::iterator it = params.begin(); it != params.end(); ++it){
    	string name = it->first;
    	string value = it->second;
    	if(name == "weight"){
    	    weight = std::atof(value.c_str());
	}
    	else if(name == "dispersion"){
    	    dispersion = std::atof(value.c_str());
	}
	else{
	    ROS_WARN("minEuclidianDistance: String %s does not name a valid parameter", name.c_str());
	}
    }
}

const char* minEuclidianDistance::name() const { return "minEuclidianDistance";}


const char* minEuclidianDistance::printInfo(const frontier& f) const
{
    std::ostringstream oss;

    oss << "Euclidian Distance: " << dist(f);

    return oss.str().c_str();
}


double minEuclidianDistance::value(const frontier& f) const
{
    double distance_value = exp( -dist(f) / dispersion );
    return distance_value*weight;
}


double minEuclidianDistance::dist(const frontier& f) const
{
    geometry_msgs::Point p = f.free_center_point;
    geometry_msgs::Point p_robot = RobotMotion::position();
    return hypot(p.x - p_robot.x, p.y - p_robot.y);
}


//--------------------------------------------------------------------------------------
//
// 	MINUMUM ASTAR DISTANCE
//
//--------------------------------------------------------------------------------------

minAStarDistance::minAStarDistance(map<string, string> params)
{
    weight = 1;
    dispersion = 1;
    for(map<string, string>::iterator it = params.begin(); it != params.end(); ++it){
    	string name = it->first;
    	string value = it->second;
    	if(name == "weight"){
    	    weight = std::atof(value.c_str());
	}
    	else if(name == "dispersion"){
    	    dispersion = std::atof(value.c_str());
	}
	else{
	    ROS_WARN("minAStarDistance: String %s does not name a valid parameter", name.c_str());
	}
    }
}

const char* minAStarDistance::name() const { return "minAStarDistance";}


const char* minAStarDistance::printInfo(const frontier& f) const
{
    std::ostringstream oss;

    oss << "AStar Distance: " << dist(f);

    return oss.str().c_str();
}


double minAStarDistance::value(const frontier& f) const
{
    double distance_value = exp( -dist(f) / dispersion );
    return distance_value*weight;
}


double minAStarDistance::dist(const frontier& f) const
{
    AStar a_star(RobotMotion::position());
    int p = f.free_center_cell;

    return a_star.distance(p);
}

} /* strategy */
} /* cam_exploration */
