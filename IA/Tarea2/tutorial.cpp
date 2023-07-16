//
//  tutorial.cpp
//  RLTutorial
//
//  Created by Julio Godoy on 11/25/18.
//  Copyright © 2018 Julio Godoy. All rights reserved.
//

#include <iostream>
#include <vector>
#include <fstream>
#include <stdlib.h>
#include <math.h>
#include <cmath>
#include <cstdlib>
#include <time.h>
#include <string.h>

using namespace std;

int height_grid, width_grid, action_taken, action_taken2, current_episode;
int maxA[100][100], blocked[100][100];
float maxQ[100][100], cum_reward, Qvalues[100][100][4], reward[100][100], finalrw[50000];
int init_x_pos, init_y_pos, goalx, goaly, x_pos, y_pos, prev_x_pos, prev_y_pos, blockedx, blockedy, i, j, k;
ofstream reward_output;

//////////////
// Setting value for learning parameters
int action_sel = 2;         // 1 is greedy, 2 is e-greedy
int environment = 1;        // 1 is small grid, 2 is Cliff walking
int algorithm = 1;          // 1 is Q-learning, 2 is Sarsa
int stochastic_actions = 1; // 0 is deterministic actions, 1 for stochastic actions
int num_episodes = 3000;     // total learning episodes
float learn_rate = 0.1;     // how much the agent weights each new sample
float disc_factor = 0.99;   // how much the agent weights future rewards
float exp_rate = 0.3;       // how much the agent explores
///////////////

void Initialize_environment()
{
    if (environment == 1)
    {

        height_grid = 3;
        width_grid = 4;
        goalx = 3;
        goaly = 2;
        init_x_pos = 0;
        init_y_pos = 0;
    }

    if (environment == 2)
    {

        height_grid = 4;
        width_grid = 12;
        goalx = 11;
        goaly = 0;
        init_x_pos = 0;
        init_y_pos = 0;
    }

    for (i = 0; i < width_grid; i++)
    {
        for (j = 0; j < height_grid; j++)
        {

            if (environment == 1)
            {
                reward[i][j] = -0.04; //-1 if environment 2
                blocked[i][j] = 0;
            }

            if (environment == 2)
            {
                reward[i][j] = -1;
                blocked[i][j] = 0;
            }

            for (k = 0; k < 4; k++)
            {
                Qvalues[i][j][k] = rand() % 10;
                cout << "Initial Q value of cell [" << i << ", " << j << "] action " << k << " = " << Qvalues[i][j][k] << "\n";
            }
        }
    }

    if (environment == 1)
    {
        reward[goalx][goaly] = 100;
        reward[goalx][(goaly - 1)] = -100;
        blocked[1][1] = 1;
    }

    if (environment == 2)
    {
        reward[goalx][goaly] = 1;

        for (int h = 1; h < goalx; h++)
            reward[h][0] = -100;
    }
}

int greedy()
{
    int maxA;             // accion con el Q mayor
    float maxQ = -999999; // Qvalue mayor

    for (int i = 0; i < 4; i++)
        if (Qvalues[x_pos][y_pos][i] > maxQ)
            maxA = i,
            maxQ = Qvalues[x_pos][y_pos][i];

    action_taken = maxA; // accion que se va tomar
    return maxA;
}
int action_selection()
{ // Based on the action selection method chosen, it selects an action to execute next

    int maxA;
    if (action_sel == 1) // Greedy, always selects the action with the largest Q value
        maxA = greedy();

    // 2.- Implemente la estrategia (policy) epsilon-greedy para la selección de acciones (con un valor de epsilon = 0.05)
    else if (action_sel == 2) // epsilon-greedy, selects the action with the largest Q value with prob (1-exp_rate) and a random action with prob (exp_rate)
    {
        const float e = 0.05;
        float random = ((float)rand()) / RAND_MAX; // Random entre 0 y 1
        // Exploracion: Se escoge una accion random
        if (random < e)
            return rand() % 4;
        // Explotacion: Se escoge la accion con el mayor valor Q
        else
            maxA = greedy();
    }

    return maxA;
}

void move(int action)
{
    prev_x_pos = x_pos; // Backup of the current position, which will become past position after this method
    prev_y_pos = y_pos;

    // Stochastic transition model (not known by the agent)
    // Assuming a .8 prob that the action will perform as intended, 0.1 prob. of moving instead to the right, 0.1 prob of moving instead to the left

    // 3.- Implemente acciones estocásticas
    if (stochastic_actions)
    {
        // Code here should change the value of variable action, based on the stochasticity of the action outcome

        float random = ((float)rand()) / RAND_MAX; // Random entre 0 y 1

        if (random < 0.1)              // Movimiento a la derecha (10% de probabilidad)
            action = (action + 1) % 4; // +1 accion derecha
        else if (random > 0.9)         // Movimiento a la izquierda (10% de probabilidad)
            action = (action + 3) % 4; // -1 accion izquierda (+3 y modulo)
    }

    // After determining the real outcome of the chosen action, move the agent

    if (action == 0)                                                         // Up
        if ((y_pos < (height_grid - 1)) && (blocked[x_pos][y_pos + 1] == 0)) // If there is no wall or obstacle Up from the agent
            y_pos = y_pos + 1;                                               // move up

    if (action == 1)                                                        // Right
        if ((x_pos < (width_grid - 1)) && (blocked[x_pos + 1][y_pos] == 0)) // If there is no wall or obstacle Right from the agent
            x_pos = x_pos + 1;                                              // Move right

    if (action == 2)                                         // Down
        if ((y_pos > 0) && (blocked[x_pos][y_pos - 1] == 0)) // If there is no wall or obstacle Down from the agent
            y_pos = y_pos - 1;                               // Move Down

    if (action == 3)                                         // Left
        if ((x_pos > 0) && (blocked[x_pos - 1][y_pos] == 0)) // If there is no wall or obstacle Left from the agent
            x_pos = x_pos - 1;                               // Move Left
}

void update_q_prev_state() // Updates the Q value of the previous state
{

    // Determine the max_a(Qvalue[x_pos][y_pos])
    float maxQ = -999999;

    for (int i = 0; i < 4; i++)
        if (Qvalues[x_pos][y_pos][i] > maxQ)
            maxQ = Qvalues[x_pos][y_pos][i];
    
    // Update the Q value of the previous state and action if the agent has not reached a terminal state
    if (!(((x_pos == goalx) && (y_pos == goaly)) || ((environment == 1) && (x_pos == goalx) && (y_pos == (goaly - 1))) || ((environment == 2) && (x_pos > 0) && (x_pos < goalx) && (y_pos == 0))))
        // Q(St,At) = Q(St,At) + alpha * [Rt+1 + gamma * maxQ(St+1,a) - Q(St,At)]
        // En este caso St = posiciones prev, At = action_taken, t+1 = actual (sin prev)
        Qvalues[prev_x_pos][prev_y_pos][action_taken] +=
            learn_rate * (reward[x_pos][y_pos] + disc_factor * maxQ - Qvalues[prev_x_pos][prev_y_pos][action_taken]);

    else // Update the Q value of the previous state and action if the agent has reached a terminal state
        Qvalues[prev_x_pos][prev_y_pos][action_taken] = Qvalues[prev_x_pos][prev_y_pos][action_taken];
}

void update_q_prev_state_sarsa()
{
    // Update the Q value of the previous state and action if the agent has not reached a terminal state
    if (!(((x_pos == goalx) && (y_pos == goaly)) || ((environment == 1) && (x_pos == goalx) && (y_pos == (goaly - 1))) || ((environment == 2) && (x_pos > 0) && (x_pos < goalx) && (y_pos == 0))))
    {

        Qvalues[prev_x_pos][prev_y_pos][action_taken] = Qvalues[prev_x_pos][prev_y_pos][action_taken];
    }
    else // Update the Q value of the previous state and action if the agent has reached a terminal state
    {
        Qvalues[prev_x_pos][prev_y_pos][action_taken] = Qvalues[prev_x_pos][prev_y_pos][action_taken];
    }
}

void Qlearning()
{
    // Follow the  steps in the pseudocode in the slides

    // Elegir A de S usando un policy: action_selection()
    // Luego, tomar accion: move()
    move(action_selection());

    // Observar R, S'
    cum_reward = cum_reward + reward[x_pos][y_pos]; // Add the reward obtained by the agent to the cummulative reward of the agent in the current episode

    // Q(S,A) = Q(S,A) + alpha * [R + gamma * maxQ(S',a) - Q(S,A)]
    update_q_prev_state();
}

void Sarsa()
{
    move(action_selection());
    cum_reward = cum_reward + reward[x_pos][y_pos]; // Add the reward obtained by the agent to the cummulative reward of the agent in the current episode
}

void Multi_print_grid()
{
    int x, y;

    for (y = (height_grid - 1); y >= 0; --y)
    {
        for (x = 0; x < width_grid; ++x)
        {

            if (blocked[x][y] == 1)
                cout << " \033[42m# \033[0m";
            else
            {
                if ((x_pos == x) && (y_pos == y))
                    cout << " \033[44m1 \033[0m";
                else
                    cout << " \033[31m0 \033[0m";
            }
        }
        printf("\n");
    }
}

int main(int argc, char *argv[])
{
    srand(time(NULL));

    reward_output.open("Rewards.txt", ios_base::out);
    Initialize_environment(); // Initialize the features of the chosen environment (goal and initial position, obstacles, rewards)

    for (i = 0; i < num_episodes; i++)
    {
        cout << "\n \n Episode " << i;
        current_episode = i;
        x_pos = init_x_pos;
        y_pos = init_y_pos;
        cum_reward = 0;

        // If Sarsa was chosen as the algorithm:
        if (algorithm == 2)
            action_taken = action_selection();

        // While the agent has not reached a terminal state:
        while (!(((x_pos == goalx) && (y_pos == goaly)) || ((environment == 1) && (x_pos == goalx) && (y_pos == (goaly - 1))) || ((environment == 2) && (x_pos > 0) && (x_pos < goalx) && (y_pos == 0))))
        {
            if (algorithm == 1)
            {

                Qlearning();
            }
            if (algorithm == 2)
            {
                Sarsa();
            }
        }

        finalrw[i] = cum_reward;
        cout << " Total reward obtained: " << finalrw[i] << "\n";
        // 1.- Imprimir en un archivo los pares [episodio, reward acumulado]
        reward_output << i << " " << finalrw[i] << endl;
    }
    reward_output.close();

    return 0;
}
