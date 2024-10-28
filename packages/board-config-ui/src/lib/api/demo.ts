import {request} from '$lib/api/request';

export const demo = {
    put: (state: boolean) => {
        return request('/api/demo', {method: 'POST', body: JSON.stringify({state})})
    },
}
