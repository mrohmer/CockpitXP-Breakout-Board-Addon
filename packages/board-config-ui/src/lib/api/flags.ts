import {request} from '$lib/api/request';

export const flags = {
    put: (state: number) => {
        return request('/api/state/flags', {method: 'POST', body: JSON.stringify({state})})
    },
}
