import {request} from '$lib/api/request';

export const demo = {
    get: async () => {
        try {
            return await request('/api/demo');
        } catch (e) {
            return {state: false};
        }
    },
    put: (state: boolean) => {
        return request('/api/demo', {method: 'POST', body: JSON.stringify({state})})
    },
}
