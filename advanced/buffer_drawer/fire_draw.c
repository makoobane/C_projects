#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "defs.h"

#define WIDTH 320
#define HEIGHT 240

static u32 framebuffer[WIDTH * HEIGHT];

/* Doom fire variables and color palette */
#define FIRE_W WIDTH
#define FIRE_H HEIGHT

#define FIRE_PALETTE_SIZE 37

static u8 fire_pixels[FIRE_W * FIRE_H];

static const u32 fire_palette[FIRE_PALETTE_SIZE] = {
  0x070707, 0x1F0707, 0x2F0F07, 0x470F07, 0x571707, 0x671F07, 0x771F07,
  0x8F2707, 0x9F2F07, 0xAF3F07, 0xBF4707, 0xC74707, 0xDF4F07, 0xDF5707,
  0xDF5707, 0xD75F07, 0xD75F07, 0xD7670F, 0xCF6F0F, 0xCF770F, 0xCF7F0F,
  0xCF8717, 0xC78717, 0xC78F17, 0xC7971F, 0xBF9F1F, 0xBF9F1F, 0xBFA727,
  0xBFA727, 0xBFAF2F, 0xB7AF2F, 0xB7B72F, 0xB7B737, 0xCFCF6F, 0xDFDF9F,
  0xEFEFC7, 0xFFFFFF
};

/* Draw a pixel in the framebuffer at 2D coordinates x and y */
void put_pixel(u32 x, u32 y, u32 color) {
  if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) {
    return;
  }
  framebuffer[y * WIDTH + x] = color;
}

/* Clear the entire framebuffer with a unique color */
void clear(u32 color) {
  for (u32 i = 0; i < WIDTH * HEIGHT; i++) {
    framebuffer[i] = color;
  }
}

/* Main function */
int main(void) {
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Texture *texture;
  SDL_Event event;

  const f64 target_frame = 1.0 / 60.0;

  if (SDL_Init(SDL_INIT_VIDEO)!=0) {
    fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }

  window = SDL_CreateWindow(
    "Framebuffer",
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    WIDTH * 2,
    HEIGHT * 2,
    0
  );

  if (window == NULL) {
    fprintf(stderr, "SDL_CreateWindow failed: %s\n", SDL_GetError());
    SDL_Quit();
    return EXIT_FAILURE;
  }

  renderer = SDL_CreateRenderer(
    window,
    -1,
    0
  );

  if (renderer == NULL) {
    fprintf(stderr, "SDL_CreateRenderer failed: %s\n", SDL_GetError());

    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_FAILURE;
  }

  texture = SDL_CreateTexture(
    renderer,
    SDL_PIXELFORMAT_XRGB8888,
    SDL_TEXTUREACCESS_STREAMING,
    WIDTH,
    HEIGHT
  );

  SDL_SetTextureScaleMode(texture, SDL_ScaleModeNearest);

  if (texture == NULL) {
    fprintf(stderr, "SDL_CreateTexture failed: %s\n", SDL_GetError());

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_FAILURE;
  }

  /* Fire initialization */
  for (int i = 0; i < FIRE_W * FIRE_H; i++) {
    fire_pixels[i] = 0;
  }

  /* Seed the fire bottom row with maximum intensity (this is the fire source) */
  for (int i = 0; i < FIRE_W; i++) {
    fire_pixels[(FIRE_H - 1) * FIRE_W + i] = FIRE_PALETTE_SIZE - 1;
  }

  u32 is_running = 1;
  u32 frame = 1;

  /* Frame loop */
  while (is_running) {
    u64 start = SDL_GetPerformanceCounter();

    /* Poll events */
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        is_running = false;
      }
    }

    clear(0x000000);

    /* Fire update */
    for (u32 x = 0; x < FIRE_W; x++) {
      for (u32 y = 1; y < FIRE_H; y++) {
        u32 src = FIRE_W * y + x;
        u32 rand_idx = rand() % 4;
        i32 dst = (i32)src - FIRE_W;
        i32 dst_x = (i32)(src % FIRE_W) - (i32)rand_idx + 1;
        if (dst < 0 || dst_x < 0 || dst_x >= FIRE_W) {
          continue;
        } else {
          dst = dst - (i32)(src % FIRE_W) + dst_x;
          u8 src_val = fire_pixels[src];
          u8 decay = rand_idx & 1;
          fire_pixels[dst] = (src_val > decay) ? (src_val - decay) : 0;
        }
      }
    }

    /* Fire render */
    for (u32 y = 0; y < FIRE_H; y++) {
      for (u32 x = 0; x < FIRE_W; x++) {
        u8 idx = fire_pixels[y * FIRE_W + x];
        put_pixel(x, y, fire_palette[idx]);
      }
    }

    /* Copy contents of the framebuffer to our SDL texture */
    SDL_UpdateTexture(
      texture,
      NULL,
      framebuffer,
      WIDTH * sizeof(u32)
    );

    /* Render texture and display the renderer */
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);

    u64 end = SDL_GetPerformanceCounter();

    double elapsed = (double)(end - start) / (double)SDL_GetPerformanceFrequency();

    /* Cap the framerate to 60 FPS (16 milliseconds each frame) */
    if (elapsed < target_frame) {
      SDL_Delay((u32)((target_frame - elapsed) * 1000.0));
    }

    frame++;
  }

  /* Free all the SDL resources we created */
  SDL_DestroyTexture(texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return EXIT_SUCCESS;
}