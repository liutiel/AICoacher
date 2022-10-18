#include "pch.h"
#include "HealthAction.h"

// Empty class
HealthAction::HealthAction()
{
	//
}

// Initialize health course from .txt configuration file
HealthAction::HealthAction(std::binary_semaphore* singal_action_end_ptr, std::string action_config_name, int action_count_limit, int action_time_limit,
    std::binary_semaphore* handle_count_refresh_ptr,
    std::binary_semaphore* handle_time_refresh_ptr)
{
    this->singal_action_end_ptr = singal_action_end_ptr;

    // Initialize handles
    this->handle_count_refresh_ptr = handle_count_refresh_ptr;
    this->handle_time_refresh_ptr = handle_time_refresh_ptr;
    
    this->count_limit = action_count_limit;
	this->time_limit = action_time_limit;

    // Read action configuration from .txt file
    std::ifstream action_config_reader(this->ACTION_FILE_DIR + action_config_name + ".txt", std::ifstream::in);

    // std::cout << this->ACTION_FILE_DIR + action_config_name + ".txt" << std::endl;

    // Read the action configuration line by line
    bool is_action_name_obtained = false;
    char action_config_line[256];
    while (action_config_reader.good())
    {
        action_config_reader.getline(action_config_line, 256);
        std::string action_config_line_string = action_config_line;
        action_config_line_string.erase(0, action_config_line_string.find_first_not_of(" "));

        if (action_config_line_string.length() == 0)
        {
            continue;
        }

        // Notes starting with '//'
        if (action_config_line_string.substr(0, 2) == "//")
        {
            continue;
        }

        // Obtain action name
        if (is_action_name_obtained == false)
        {
            this->action_name = action_config_line_string;
            is_action_name_obtained = true;
        }
        else
        {
            // Read the line to get the details of the key frames
            std::stringstream course_config_line_string_stream;
            course_config_line_string_stream << action_config_line_string;

            std::string action_keyframe_content_string = "";
            std::vector<int> action_keyframe_complete;
            std::vector<int> action_keyframe_perfect;
            bool is_perfect_content = false;

            while(true)
            {
                if(!(course_config_line_string_stream >> action_keyframe_content_string)) break;

                if (action_keyframe_content_string == "") break;
                else
                {
                    if (action_keyframe_content_string == "/")
                        is_perfect_content = true;
                    else
                    {
                        int action_keyframe_content_int = atoi(action_keyframe_content_string.c_str());
                        if (!is_perfect_content)
                        {
                            action_keyframe_complete.push_back(action_keyframe_content_int);
                        }
                        else
                        {
                            action_keyframe_perfect.push_back(action_keyframe_content_int);
                        }
                    }
                }
            }

            this->key_frame_list_complete.push_back(action_keyframe_complete);
            this->key_frame_list_perfect.push_back(action_keyframe_perfect);
        }
    }
    action_config_reader.close();

    // std::cout << this->action_name << std::endl;

}

void HealthAction::startActionTimer()
{
    this->action_timer = std::thread(&HealthAction::secondTimer, this);
    this->action_timer.detach();

    this->is_action_active = true;
}

void HealthAction::secondTimer()
{
    do 
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        if (!this->is_action_active)
        {
            continue;
        }
        this->current_time++;
        this->handle_time_refresh_ptr->release();

        // std::cout << this->current_time << std::endl;

        if (this->time_limit != 0 && current_time >= this->time_limit)
        {
            // std::cout << "FLAG: TIME LIMIT REACH! " << std::endl;
            this->singal_action_end_ptr->release();
            this->isActionEnded = true;
            break;
        }
    } while (!this->isActionEnded);

}

void HealthAction::actionCountOnce()
{
    this->current_count++;

    this->current_perfection = this->current_perfect_frame_index >= key_frame_list_perfect.size();

    this->handle_count_refresh_ptr->release();
    // std::cout << "COUNT!" << std::endl;

    if (this->count_limit != 0 && this->current_count >= this->count_limit)
    {
        this->isActionEnded = true;
        // std::cout << "FLAG: COUNT LIMIT REACH! " << std::endl;
        this->singal_action_end_ptr->release();
    }
}

bool HealthAction::actionTransferCheck(std::array<std::array<double, 2>, PoseDetector::DETECTION_MODEL_OUTPUT_CHANNEL> person_pose)
{
    std::vector<int> current_action_state = this->getCurrentActionState(person_pose);

    if (this->actionVectorCompare(key_frame_list_complete[current_key_frame_index], current_action_state))
    {
        this->current_key_frame_index++;

        if (this->current_perfect_frame_index < key_frame_list_perfect.size())
        {
            if (this->actionVectorCompare(key_frame_list_perfect[current_perfect_frame_index], current_action_state))
            {
                this->current_perfect_frame_index++;
            }
        }

        if (this->current_key_frame_index >= key_frame_list_complete.size())
        {
            this->actionCountOnce();

            this->current_key_frame_index = 0;
            this->current_perfect_frame_index = 0;
        }
    }

}

// Pause or resume action
void HealthAction::setActionActive(bool input_action_state)
{
    this->is_action_active = input_action_state;
}

// Stop action from external
void HealthAction::stopAction()
{
    this->isActionEnded = true;
}

std::vector<int> HealthAction::getCurrentActionState(std::array<std::array<double, 2>, PoseDetector::DETECTION_MODEL_OUTPUT_CHANNEL> person_pose)
{
    std::vector<int> current_action_state;
    current_action_state.push_back(WorkoutParameters::IsHandOverShoulder(person_pose)); //0.
    current_action_state.push_back(WorkoutParameters::IsSide(person_pose)); //1.
    current_action_state.push_back(WorkoutParameters::CosKnee(person_pose)); //2.
    current_action_state.push_back(WorkoutParameters::IsKneeRaisedR(person_pose)); //3.
    current_action_state.push_back(WorkoutParameters::IsKneeRaisedL(person_pose)); //4.
    current_action_state.push_back(WorkoutParameters::IsSameSideShoulderElbowR(person_pose)); //5.
    current_action_state.push_back(WorkoutParameters::IsSameSideShoulderElbowL(person_pose)); //6.
    current_action_state.push_back(WorkoutParameters::IsAnkleRaisedR(person_pose)); //7.
    current_action_state.push_back(WorkoutParameters::IsAnkleRaisedL(person_pose)); //8.
    current_action_state.push_back(WorkoutParameters::IsKneeOverHand(person_pose)); //9.
    current_action_state.push_back(WorkoutParameters::IsArmStraightV(person_pose)); //10.
    current_action_state.push_back(WorkoutParameters::IsArmStraightL(person_pose)); //11.
    current_action_state.push_back(WorkoutParameters::IsAnkleClose(person_pose)); //12.
    current_action_state.push_back(WorkoutParameters::IsLegUpright(person_pose)); //13.
    current_action_state.push_back(WorkoutParameters::IsHandOverHead(person_pose)); //14.
    current_action_state.push_back(WorkoutParameters::IsElbowHigherR(person_pose)); //15.
    current_action_state.push_back(WorkoutParameters::IsElbowHigherL(person_pose)); //16.

    return current_action_state;
}

bool HealthAction::actionVectorCompare(std::vector<int>key_frame_vector, std::vector<int> current_vector)
{
    int compare_length = std::min(key_frame_vector.size(), current_vector.size());

    for (int i = 0; i < compare_length; i++)
    {
        if (key_frame_vector[i] == -1) continue;
        if (key_frame_vector[i] != current_vector[i]) return false;
    }

    return true;
}

/*Getters*/
std::string HealthAction::getActionName() { return this->action_name; }
int HealthAction::getActionCount() { return this->current_count; }
int HealthAction::getActionTime() { return this->current_time; }
int HealthAction::getActionCountLimit() { return this->count_limit; }
int HealthAction::getActionTimeLimit() { return this->time_limit; }
int HealthAction::getActionPerfection() { return this->current_perfection; }