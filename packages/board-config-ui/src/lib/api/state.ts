import {request} from '$lib/api/request';

export const state = {
    get: async () => {
        try {
            return await request('/api/state');
        } catch (e) {
            return {demo: false, pitlane: {lane1: false, lane2: false}, flags: 0};
        }
    },
}
