#include <iostream>
#include <map>
#include <string>
#include <future>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "shader_s.h"
#include "SongLoader.cpp"


#include	<cstdio>
#include	<cstring>

#include	<sndfile.hh>

#include <windows.h>
#include <mmsystem.h>

#include <portaudio.h>

#include "SoundManager.cpp"


#define		BUFFER_LEN		1024

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void RenderText(Shader& shader, std::string text, float textCenter, float y, float scale, glm::vec3 color, glm::vec3 colorHighlight, float highlightIndex);

string generateString(Line line);
// settings
const unsigned int WINDOW_WIDTH = 1280;
const unsigned int WINDOW_HEIGHT = 720;
const float WINDOW_CENTER_Y = WINDOW_HEIGHT / 2.0f;
float WINDOW_CENTER_X = WINDOW_WIDTH / 2.0f;

const float LINE_HEIGHT = 75.0f;

/// Holds all state information relevant to a character as loaded using FreeType


unsigned int VAO, VBO;
int letter = 0;
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

GlyphManager glyphs;






int main()
{

    std::cout << "STARTING..." << std::endl;

    SoundManager soundManager;
    soundManager.init();

    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenSing", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // OpenGL state
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);



    // compile and setup the shader
    Shader shader("shaders/text.vs", "shaders/text.fs");
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(WINDOW_WIDTH), 0.0f, static_cast<float>(WINDOW_HEIGHT));
    shader.use();
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));


    // configure VAO/VBO for texture quads
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glyphs.initGlyphs();

    // load song from .sing file
    SongLoader songLoader;
    Song song = songLoader.loadSong("songs/elvis.sing");

    soundManager.loadSound("songs/elvis.wav");


    unsigned short currState = 0;


    bool firstFrame = true;

    const double COUNTDOWN = 3.0f;

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {


        // time

        float currentFrame = glfwGetTime();
        float songTimestamp = currentFrame - COUNTDOWN;
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;          
        // input

        processInput(window);
        shader.setFloat("alpha", 1.0f);

        // countdown from COUNTDOWN state
        if (currentFrame < COUNTDOWN) {

            if (currentFrame > COUNTDOWN - 0.8f) {

                glm::vec3 firstPageColor = glm::vec3(song.getPages()[0].getBgColor().x, song.getPages()[0].getBgColor().y, song.getPages()[0].getBgColor().z);
                float percent =  (currentFrame- COUNTDOWN - 0.8f) / 0.5f;
                if (percent > 1.0f) {
                    percent = 1.0f;
                }


                glClearColor(firstPageColor.x * percent, firstPageColor.y * percent, firstPageColor.z * percent, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);
            }
            else {
                glClearColor(0, 0, 0, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);
            }


            if (ceil(currentFrame) - (currentFrame) < 0.5f) {
                shader.setFloat("alpha", 2.0 * (ceil(currentFrame) - currentFrame));
            }
            RenderText(shader, to_string(ceil(COUNTDOWN - currentFrame)).substr(0, 1), glyphs.getPixelLength("5"), WINDOW_HEIGHT / 2.0f, 3.0 * (ceil(currentFrame) - currentFrame), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 0.9f, 0.1f), 0);
            
            if (currentFrame < COUNTDOWN - 0.5f) {
                shader.setFloat("alpha", 1.0f);
            }
            RenderText(shader, song.getTitle(), glyphs.getPixelLength(song.getTitle()), 25.0f, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f), song.getHighlightColor(), 0.0f);

        }
        // song playback state
        else if (song.getLength() + COUNTDOWN > currentFrame && currentFrame > COUNTDOWN) {

            if (firstFrame == true) {
                firstFrame = false;

                soundManager.playSound();
            }

            // checks
            song.updatePage(songTimestamp);

            Page& page = song.getPage();



            // render

            glClearColor(page.getBgColor().x, page.getBgColor().y, page.getBgColor().z, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            RenderText(shader, to_string(songTimestamp).substr(0, 5), glyphs.getPixelLength(to_string(songTimestamp).substr(0, 5)), 700.0f, 0.3f, glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 0.9f, 0.1f), 0.0f);



            //vertical centering

            int lineNumber = page.getLines().size();

            float pageCenterHeight = (lineNumber * LINE_HEIGHT) / 2.0f;
            float offset = -glyphs.Characters['E'].Bearing.y;
            float y = WINDOW_CENTER_Y + pageCenterHeight + offset;

            int currLineIndex = page.getLineNumber();
            for (int i = 0; i < page.getLines().size(); i++) {

                float highlightIndex;


                if (i < currLineIndex)
                    highlightIndex = -1.0f;
                else if (i == currLineIndex) {
                    highlightIndex = page.getLines()[i].getLengthToHighlight(songTimestamp);
                }
                else {
                    highlightIndex = 0.0f;
                }


                RenderText(shader, generateString(page.getLines()[i]), page.getLines()[i].getPixelWidth(), y, .75f, song.getTextColor(), song.getHighlightColor(), highlightIndex);
                y -= LINE_HEIGHT;
            }
        }
        // song over state
        else {
            glClearColor(0, 0, 0, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            RenderText(shader, "Song is over!", glyphs.getPixelLength("Song is over!"), WINDOW_CENTER_Y, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 0.9f, 0.1f), 0);


        }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    soundManager.stopSound();

    glfwTerminate();
    return 0;
}


string generateString(Line line) {
    string renderTxt = "";

    for (int i = 0; i < line.getSyllables().size(); i++) {
        renderTxt.append(line.getSyllables()[i].getSyllable());

    }

    return renderTxt;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        letter++;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// render line of text
// -------------------
void RenderText(Shader& shader, std::string text, float x, float y, float scale, glm::vec3 color, glm::vec3 colorHighlight, float highlightIndex)
{
    // activate corresponding render state	
    shader.use();
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);


    float textCenter = x * scale / 2.0f;



    float ogX = WINDOW_CENTER_X - textCenter;

    glUniform3f(glGetUniformLocation(shader.ID, "highlightColor"), colorHighlight.x, colorHighlight.y, colorHighlight.z);
    glUniform3f(glGetUniformLocation(shader.ID, "textColor"), color.x, color.y, color.z);



    // iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {


        shader.setFloat("percentToHighlight", highlightIndex);


        GlyphManager::Character ch = glyphs.Characters[*c];

        float xpos = ogX + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;
        // update VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };
        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        ogX += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))

        if (highlightIndex > 0) {
            highlightIndex--;
        }

    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

}

