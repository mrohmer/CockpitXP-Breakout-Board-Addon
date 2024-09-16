/** @type {import('tailwindcss').Config} */
export default {
  content: ['./src/**/*.{html,js,svelte,ts}'],
  theme: {
    extend: {}
  },
  plugins: [
    require('daisyui'),
  ],
  daisyui: {
    themes: [
      {
        cma: {
          "primary": "#cb9b51",
          "secondary": "#263e3f",
          "accent": "#0f576d",
          "neutral": "#3d4451",
          "base-100": "#000",
          "base-200": "#171717",
        },
      },],
  },
};
