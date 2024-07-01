import {request} from '$lib/api/request';

export const demo = {
    get: async () => {
        try {
            return await request('/api/demo');
        } catch (e) {
            return {active: false};
        }
    },
    put: (state: boolean) => {
        if (state) {
            return request('/api/demo/activate', {method: 'POST', body: JSON.stringify({state})})
        }
        return request('/api/demo/deactivate', {method: 'POST', body: JSON.stringify({state})})
    },
}
