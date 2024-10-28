import {request} from '$lib/api/request';

export const pitlanes = {
    put: (lane1: boolean, lane2: boolean) => {
        return request('/api/state/pitlanes', {method: 'POST', body: JSON.stringify({lane1, lane2})})
    },
}
