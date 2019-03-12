import './bootstrap.js';

import Vue from 'vue';
import App from './components/App';
import axios from "axios";

new Vue({
    el: '#app',
    render: h => h(App)
});
