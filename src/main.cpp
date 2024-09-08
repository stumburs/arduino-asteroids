#include <Arduino.h>
#include <TVout.h>
#include <font6x8.h>

// UNCOMMENT TO ENABLE

// Limit FPS to 60
// #define FPS_LIMIT

TVout TV;

int leftHorizPin = A0; // Potentiometer output connected to analog pin 3
int leftHoriz = 0;

constexpr int render_width = 120;
constexpr int render_height = 96;

const int target_fps = 60;                              // Target frames per second
const unsigned long frame_duration = 1000 / target_fps; // Duration of each frame in milliseconds

unsigned long previous_time = 0; // To store the time of the last frame
size_t frame_count = 0;          // To count frames for FPS calculation
unsigned long fps_last_time = 0; // To store the last time FPS was calculated
size_t fps = 0;                  // The calculated FPS value

void setup()
{
    TV.begin(PAL, render_width, render_height);
    TV.select_font(font6x8);
}

void loop()
{
    unsigned long current_time = millis(); // Get the current time in milliseconds

#ifdef FPS_LIMIT
    // Check if it's time to render the next frame
    if (current_time - previous_time >= frame_duration)
    {
#endif
        previous_time = current_time; // Update the last frame time
        leftHoriz = analogRead(leftHorizPin);
        TV.clear_screen();
        TV.draw_circle(render_width / 2, render_height / 2, 20, 1, -1);
        TV.draw_rect(map(leftHoriz, 0, 1024, render_width / 2 + 20, render_width / 2 - 20), render_height / 2, 2, 2, 1, 1);

        // Increment the frame count
        frame_count++;

        // Calculate FPS every second
        if (current_time - fps_last_time >= 1000)
        {
            fps = frame_count;            // Set FPS to the number of frames counted
            frame_count = 0;              // Reset frame count for the next second
            fps_last_time = current_time; // Update the last FPS calculation time
        }

        TV.print("FPS: ");
        TV.print(fps); // Print the FPS value to the Serial Monitor
#ifdef FPS_LIMIT
    }
#endif
}
