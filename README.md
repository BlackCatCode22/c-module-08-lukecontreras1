[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/p7uXXSgg)
# tCppModule08spr25
tCppModule08spr25

Final Programming Assignment: Enhancing Search Algorithms in C++

Objective: Enhance and expand the given C++ program that implements an OpenAI Chatbot.

Starter Code: Zip file with root directory named chatBot01. 

Extended Point Value: Note that this final programming assignment is worth 50 points.  

Tasks:

1. Add Chat Iteration Count and Conversation History

Description: Modify the chatbot program to count and display the number of user-chatbot exchanges during the session. Additionally, maintain a conversation history that stores both the user inputs and chatbot responses, displaying it after each interaction.

Purpose: This task helps students understand how to track and manage program state over multiple iterations and provides practice with data structures such as vectors for maintaining the conversation history.

2. Implement a Recursive Function to Handle API Call Errors

Description: Write a recursive function to handle potential API call errors, such as network timeouts or rate limits. The function should attempt to resend the request up to a maximum number of retries, with a short delay between retries.

Purpose: Introduces students to error recovery strategies in programming and demonstrates practical use of recursion for handling repeated tasks with constraints.

3. Error Handling for User Inputs

Description: Add error handling to manage invalid user inputs. For example:

Check if the user input is empty.
Ensure the input does not exceed a certain character limit.
Display an error message and prompt the user to re-enter valid input when an error is detected.

Purpose: Teaches students about robust program design and ensures that the chatbot can handle unexpected or incorrect inputs gracefully.

4. Performance Measurement

Description: Implement functionality to time how long each API request takes to execute and report these times to the user. Include functionality to calculate the average response time over the course of the conversation.

Purpose: Provides practical insight into real-world API performance and demonstrates how external factors (e.g., network latency) affect response times.

Expected Outcomes

By the end of this assignment, students should be able to:

Use the OpenAI API with curl commands in C++ to send user queries and receive chatbot responses.
Maintain and manipulate program state to manage conversation history and interaction counts.
Handle errors effectively, both in user inputs and API interactions.
Measure and analyze program performance, applying real-world debugging and optimization practices.
Starter Code

The provided starter code (chatBot01) initializes a basic chatbot program using the OpenAI API. It includes:

Boilerplate for curl setup.
A function to send a user query to the API and parse the JSON response.
Basic error handling for API authentication.
Submission Guidelines

Students should submit a single C++ source file containing their completed chatbot program to the Module 08 GitHub Classroom remote repository. Ensure that your code is well-commented to explain your logic and any changes made to the starter code.

Assessment Criteria
Correctness:
The chatbot interacts with the OpenAI API as expected.
Conversation history and interaction counts are implemented correctly.
Error Handling:
The program gracefully handles invalid user inputs and API call errors.
Recursive error handling works as intended.
Performance Reporting:
Response times are accurately measured and displayed to the user.
Average response time calculation is correct.
Code Quality:
Code is clean, well-organized, and adequately commented.
Additional Notes

This assignment introduces students to real-world applications of C++ in integrating external APIs and working with JSON data. The tasks encourage critical thinking about robust program design and provide valuable experience in modern programming practices.
