import type {PageLoad} from "./$types";
import {demo} from '$lib/api/demo';

export const load: PageLoad = async ({depends}) => {
  const [d] = await Promise.all([
    demo.get(),
  ]);

  depends('index:demo');

  return {
    demo: d,
  }
}
