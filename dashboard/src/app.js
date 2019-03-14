import "./bootstrap.js";

import Vue from "vue";
import App from "./components/App";
import axios from "axios";
import Vuetify from "vuetify";
import "vuetify/dist/vuetify.min.css";

Vue.use(Vuetify);

new Vue({
  el: "#app",
  render: h => h(App)
});
