#include "pch.h"
#include "HealthCourse.h"

// Empty class
HealthCourse::HealthCourse()
{
	//
}

// Initialize health course from .txt configuration file
HealthCourse::HealthCourse(std::binary_semaphore* singal_course_end_ptr, std::string course_config_name, 
    std::binary_semaphore* handle_action_refresh_ptr,
    std::binary_semaphore* handle_count_refresh_ptr,
    std::binary_semaphore* handle_time_refresh_ptr)
{
    // Load the aicoacher object as environment
    this->singal_course_end_ptr = singal_course_end_ptr;

    // Initialize handles
    this->handle_action_refresh_ptr = handle_action_refresh_ptr;
    this->handle_count_refresh_ptr = handle_count_refresh_ptr;
    this->handle_time_refresh_ptr = handle_time_refresh_ptr;

    // Read course configuration from .txt file
    std::ifstream course_config_reader(this->COURSE_FILE_DIR + course_config_name + ".txt", std::ifstream::in);

    // Read the course configuration line by line
    bool is_course_name_obtained = false;
    char course_config_line[256];
    while (course_config_reader.good())
    {
        course_config_reader.getline(course_config_line, 256);
        std::string course_config_line_string = course_config_line;
        course_config_line_string.erase(0, course_config_line_string.find_first_not_of(" "));
        
        if (course_config_line_string.length() == 0)
        {
            continue;
        }

        // Notes starting with '//'
        if (course_config_line_string.substr(0, 2) == "//")
        {
            continue;
        }

        // Obtain course name
        if (is_course_name_obtained == false)
        {
            this->course_name = course_config_line_string;
            is_course_name_obtained =true;
        }
        else
        {
            // Read the line to create the action object
            std::stringstream course_config_line_string_stream;
            course_config_line_string_stream << course_config_line_string;

            std::string current_health_action_name = "";
            int current_health_action_limit_count = 0;
            int current_health_action_limit_time = 0;

            HealthActionInfomation current_health_action_information;

            course_config_line_string_stream >> current_health_action_information.name;
            course_config_line_string_stream >> current_health_action_information.limit_count;
            course_config_line_string_stream >> current_health_action_information.limit_time;

            this->health_action_list.push_back(current_health_action_information);
        }
    }
    course_config_reader.close();
}

bool HealthCourse::startHealthAction(int new_action_index)
{
    if (!this->is_course_started)
    {
        this->thread_internal_action_shift = std::thread(&HealthCourse::internalNextHealthAction, this);
        this->thread_internal_action_shift.detach();

        this->is_course_started = true;
    }
    
    if (new_action_index < this->health_action_list.size())
    {
        current_health_action->~HealthAction();

        current_health_action = new HealthAction(this->singal_action_end_ptr, this->health_action_list[new_action_index].name,
            this->health_action_list[new_action_index].limit_count,
            this->health_action_list[new_action_index].limit_time,
            this->handle_count_refresh_ptr,
            this->handle_time_refresh_ptr);
        current_health_action->startActionTimer();
        this->handle_action_refresh_ptr->release();

        current_health_action_index = new_action_index;

        return true;
    }
    return false;
}

// Shift to the next health action
bool HealthCourse::nextHealthAction()
{
    if (current_health_action_index >= this->health_action_list.size() - 1)
    {
        this->singal_course_end_ptr->release();
        return true;
    }
    else
    {
        this->startHealthAction(current_health_action_index + 1);
        return false;
    }
}

// Shift to the next health action when an action is ended
bool HealthCourse::internalNextHealthAction()
{
    do
    {
        this->singal_action_end_ptr->acquire();
        this->is_course_ended = this->nextHealthAction();
    } while (!this->is_course_ended);

    return true;
}

// Receive the current frame and judge whether to transfer to the next key frame
bool HealthCourse::actionTransferCheck(std::array<std::array<double, 2>, PoseDetector::DETECTION_MODEL_OUTPUT_CHANNEL> person_pose)
{
    return this->current_health_action->actionTransferCheck(person_pose);
}

// Pause or resume action
void HealthCourse::setCourseActive(bool input_course_state)
{
    
    this->current_health_action->setActionActive(input_course_state);
}

void HealthCourse::stopCourse()
{
    this->current_health_action->stopAction();
    this->is_course_ended = true;
}

// Getters
std::string HealthCourse::getCourseName() { return this->course_name; }
std::string HealthCourse::getActionName() { return this->current_health_action->getActionName(); }
//HealthAction* HealthCourse::getCurrentHealthActionPtr() { return this->current_health_action;}
int HealthCourse::getActionCount() { return this->current_health_action->getActionCount(); }
int HealthCourse::getActionTime() { return this->current_health_action->getActionTime(); }
int HealthCourse::getActionCountLimit() { return this->current_health_action->getActionCountLimit(); }
int HealthCourse::getActionTimeLimit() { return this->current_health_action->getActionTimeLimit(); }
int HealthCourse::getActionPerfection() { return this->current_health_action->getActionPerfection(); }