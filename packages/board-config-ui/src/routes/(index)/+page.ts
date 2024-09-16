import type {PageLoad} from "./$types";
import { api } from "$lib/api";

export const load: PageLoad = async ({depends}) => {
  const [d] = await Promise.all([
    api.demo.get(),
  ]);

  depends('index:demo');

  return {
    demo: d,
  }
}
