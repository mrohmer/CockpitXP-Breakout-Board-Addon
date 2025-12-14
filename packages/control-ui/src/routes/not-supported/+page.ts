import type {PageLoad} from "./$types";
import {bluetooth} from "$lib";
import {redirect} from "@sveltejs/kit";
import {browser} from "$app/environment";

export const load: PageLoad = () => {
    if (browser && bluetooth.isSupported()) {
        throw redirect(302, '/');
    }

    return {};
}