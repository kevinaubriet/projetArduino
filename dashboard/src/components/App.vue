<template>
  <dashboard columns="10" rows="8">
    <value-tile position="a1:b2" heading="Barnsley" color="blue">
      <date-time slot="before" format="ddd DD/MM" time-zone="Europe/Paris"></date-time>
      <date-time slot="value" format="HH:mm" time-zone="Europe/Paris"></date-time>
      <weather slot="after" city="Barnsley"></weather>
    </value-tile>

    <indicator-tile position="a3:b3" :value="etatLed" color="green" label="Led Arduino"></indicator-tile>

    <tile position="c3:d3">
      <v-btn large @click="modifEtatButton" v-show="!etatLed">Allumer</v-btn>
      <v-btn large @click="modifEtatButton" v-show="etatLed">Eteindre</v-btn>
    </tile>

    <value-tile
      position="c1:d2"
      heading="Temperature"
      color="yellow"
      :value="temperature"
      :decimal-places="2"
      unit="°C"
    >
      <percentile-change slot="after" :value="temperatureMqtt"></percentile-change>
    </value-tile>
    <!-- 
    <gauge-tile
      position="c4:d5"
      color="green"
      :value="temperature"
      :max="30"
      :decimal-places="2"
      unit="°C"
    ></gauge-tile>
    -->
    <value-tile
      position="e1:f2"
      heading="Lumiere"
      color="yellow"
      :value="lumiere"
      :decimal-places="2"
      unit="lum"
    >
      <percentile-change slot="after" :value="lumiereMqtt"></percentile-change>
    </value-tile>
    <level-tile
      position="e3:e5"
      color="yellow"
      label="Temperature"
      :max="40"
      :value="temperature"
      unit="°C"
    ></level-tile>
    <!--
    <level-tile
      position="f3:f5"
      color="green"
      label="Battery"
      :min="0"
      :max="100"
      :value="battery"
      unit="%"
    ></level-tile>
    -->
    <list-tile position="i1:j5" heading="Datas" color="red" :values="datas"></list-tile>
    <list-tile position="g1:h5" heading="Datas" color="red" :values="datas"></list-tile>
    <!--
    <text-tile position="g4:h5" value="Lorem ipsum dolar sit amet consectetur adipiscing elit"></text-tile>
    
    <value-tile position="i1:j2" heading="SNR" color="orange" :value="snr" unit="dB">
      <span slot="after">
        <i class="fa fa-caret-down color--white"></i>
        <min-value :value="snr"></min-value>&nbsp;|&nbsp;
        <i class="fa fa-caret-up color--white"></i>
        <max-value :value="snr"></max-value>&nbsp;|&nbsp;
        <i class="fa fa-sort color--white"></i>
        <ema-value :value="snr"></ema-value>
      </span>
    </value-tile>
    
    <chart-tile position="i3:j5" :data="chartData3" type="doughnut"></chart-tile>
    -->
    <chart-tile
      :key="componentKeyBar"
      position="a6:f8"
      heading="Something"
      color="red"
      :data="datasForGraph"
      type="bar"
    ></chart-tile>
    <chart-tile position="g6:j8" heading="Something" color="blue" :data="datasForGraph" type="line"></chart-tile>
  </dashboard>
</template>

<script>
//"infoTemperature[0].valeur"
import Dashboard from "./Dashboard";
import Mqtt from "../mqtt.js";

import DateTime from "./atoms/DateTime";
import PercentileChange from "./atoms/PercentileChange";
import MinValue from "./atoms/MinValue";
import MaxValue from "./atoms/MaxValue";
import AvgValue from "./atoms/AvgValue";
import EmaValue from "./atoms/EmaValue";
import Weather from "./atoms/Weather";

import ValueTile from "./ValueTile";
import ListTile from "./ListTile";
import SparklineTile from "./SparklineTile";
import GaugeTile from "./GaugeTile";
import ChartTile from "./ChartTile";
import IndicatorTile from "./IndicatorTile";
import BatteryTile from "./BatteryTile";
import LevelTile from "./LevelTile";
import TextTile from "./TextTile";
import DataSerice from "../services/DataService";
import Tile from "./atoms/Tile";
import axios from "axios";
import Vue from "vue";

const dataService = new DataSerice();
const publishTopic = "m1/miage/ab/test3";
const subscribeTopicTemp = "m1/miage/ab/temperature";
const subscribeTopicLum = "m1/miage/ab/lumiere";

export default {
  components: {
    Dashboard,
    DateTime,
    PercentileChange,
    MinValue,
    MaxValue,
    AvgValue,
    EmaValue,
    Weather,
    ValueTile,
    ListTile,
    SparklineTile,
    GaugeTile,
    ChartTile,
    IndicatorTile,
    BatteryTile,
    LevelTile,
    TextTile,
    Tile
  },
  methods: {
    async update() {
      let result = await dataService.getData();
      console.log(result);
    },
    getDatas() {
      let url = "http://localhost:8081/api/datas";
      fetch(url, {
        method: "GET"
      })
        .then(responseJSON => {
          return responseJSON.json();
        })
        .then(responseJS => {
          return responseJS.data;
        })
        .then(data => {
          //console.log(data[0].valeur);
          //this.temperatures = data;
          var taille = Object.values(data).length;
          this.datas = [];
          data.forEach(element => {
            this.datas.push({
              label: element.type + " : ",
              value: element.valeur
            });
          });
          this.datas = this.datas.reverse();

          //console.log("tab datas : " + this.datas.length);
        })
        .catch(err => {
          console.log("une erreur est intervenue");
        });
    },
    getTemperatures() {
      let url = "http://localhost:8081/api/datas/temperature";
      fetch(url, {
        method: "GET"
      })
        .then(responseJSON => {
          return responseJSON.json();
        })
        .then(responseJS => {
          return responseJS.data;
        })
        .then(data => {
          //console.log(data[0].valeur);
          //this.temperatures = data;
          this.apiFormatForGraph(data);

          var taille = Object.values(data).length;
          this.temperature = parseFloat(data[taille - 1].valeur);

          this.temperatures = [];
          data.forEach(element => {
            this.temperatures.push({
              label: "temperature",
              value: element.valeur
            });
          });

          this.apiFormatForGraph(data);
          this.forceRerender();
          /*
          console.log("tableau plus haut");
          console.log(this.datasForGraph);*/
          //console.log("tab temp : " + this.temperatures.length);
        })
        .catch(err => {
          console.log("une erreur est intervenue");
        });
    },
    createData(valeurData, typeData) {
      let url = "http://localhost:8081/api/datas";

      var details = {
        valeur: valeurData,
        type: typeData
      };

      var formBody = [];
      for (var property in details) {
        var encodedKey = encodeURIComponent(property);
        var encodedValue = encodeURIComponent(details[property]);
        formBody.push(encodedKey + "=" + encodedValue);
      }
      formBody = formBody.join("&");

      fetch(url, {
        method: "POST",
        headers: {
          "Content-Type": "application/x-www-form-urlencoded"
        },
        body: formBody
      })
        .then(responseJSON => {
          return responseJSON.json();
        })
        .then(responseJS => {
          console.log(responseJS);
          return responseJS.data;
        })
        .then(data => {
          console.log(data);
          this.getTemperatures();
          this.getLumieres();
          //this.getIdLed();
          //console.log(data[0].valeur);
          //this.temperature = parseFloat(data[0].valeur);
        })
        .catch(err => {
          console.log("une erreur est intervenue");
        });
    },
    getLumieres() {
      let url = "http://localhost:8081/api/datas/lumiere";
      fetch(url, {
        method: "GET"
      })
        .then(responseJSON => {
          return responseJSON.json();
        })
        .then(responseJS => {
          return responseJS.data;
        })
        .then(data => {
          var taille = Object.values(data).length;
          this.lumiere = parseFloat(data[taille - 1].valeur);

          this.lumieres = [];
          data.forEach(element => {
            this.lumieres.push({
              label: "lumiere",
              value: element.valeur
            });
          });

          //console.log("tab lum : " + this.lumieres.length);
        })
        .catch(err => {
          console.log("une erreur est intervenue");
        });
    },
    createEtatLed() {
      let url = "http://localhost:8081/api/led";

      fetch(url, {
        method: "POST"
      })
        .then(responseJSON => {
          return responseJSON.json();
        })
        .then(responseJS => {
          return responseJS.data;
        })
        .then(data => {
          this.getIdLed();
          //console.log(data[0].valeur);
          //this.temperature = parseFloat(data[0].valeur);
        })
        .catch(err => {
          console.log("une erreur est intervenue");
        });
    },
    setEtatLedFalse() {
      let url = "http://localhost:8081/api/led/" + this.idLed;

      var details = {
        valeur: false
      };

      var formBody = [];
      for (var property in details) {
        var encodedKey = encodeURIComponent(property);
        var encodedValue = encodeURIComponent(details[property]);
        formBody.push(encodedKey + "=" + encodedValue);
      }
      formBody = formBody.join("&");

      fetch(url, {
        method: "PUT",
        headers: {
          "Content-Type": "application/x-www-form-urlencoded"
        },
        body: formBody
      })
        .then(responseJSON => {
          return responseJSON.json();
        })
        .catch(err => {
          console.log(err);
        });
    },
    getIdLed() {
      let url = "http://localhost:8081/api/led";
      fetch(url, {
        method: "GET"
      })
        .then(responseJSON => {
          return responseJSON.json();
        })
        .then(responseJS => {
          return responseJS.data;
        })
        .then(data => {
          this.idLed = data[0]._id;
          this.etatLed = data[0].valeur;
          //console.log(this.idLed);
          this.setEtatLedFalse();
        })
        .catch(err => {
          console.log("une erreur est intervenue");
        });
    },
    modifEtatButton() {
      this.etatLed = !this.etatLed;
      this.modifEtatLed();
      var objectLed = { valeur: this.etatLed, _id: this.idLed };
      Mqtt.publish(publishTopic, objectLed);
    },
    modifEtatLed() {
      let url = "http://localhost:8081/api/led/" + this.idLed;

      var details = {
        valeur: this.etatLed
      };

      var formBody = [];
      for (var property in details) {
        var encodedKey = encodeURIComponent(property);
        var encodedValue = encodeURIComponent(details[property]);
        formBody.push(encodedKey + "=" + encodedValue);
      }
      formBody = formBody.join("&");

      fetch(url, {
        method: "PUT",
        headers: {
          "Content-Type": "application/x-www-form-urlencoded"
        },
        body: formBody
      })
        .then(responseJSON => {
          return responseJSON.json();
        })
        .catch(err => {
          console.log(err);
        });
    },
    apiFormatForGraph(tab) {
      var labelsO = [];
      var dataTemp = [];

      //var dataLum = [];

      tab.forEach(element => {
        dataTemp.push(parseInt(element.valeur));
        labelsO.push(this.convertDate(element.time));
      });
      /*
      this.lumieres.forEach(element => {
        dataLum.push(element.value);
      });*/

      var datasetsO = [
        {
          label: "Temperature",
          color: "#2ecc71",
          data: dataTemp
        } /*,
        { label: "Lumiere", color: "#3498db", data: dataLum }
        */
      ];

      this.datasForGraph = {
        datasets: datasetsO,
        labels: labelsO
      };
    },
    forceRerender() {
      this.componentKeyBar += 1;
      this.componentKeyLine += 1;
    },
    convertDate(dateapi) {
      var date = new Date(dateapi);
      return (
        date.getDate() +
        "/" +
        (date.getMonth() + 1) +
        "/" +
        date.getFullYear() +
        " " +
        date.getHours() +
        ":" +
        date.getMinutes()
      );
    }
  },

  data() {
    return {
      componentKeyBar: 0,
      componentKeyLine: 0,
      datas: [],
      datasForGraph: {},
      temperatures: [],
      temperature: 0,
      temperatureMqtt: 0,
      lumieres: [],
      lumiere: 0,
      lumiereMqtt: 0,
      temperatureToChart: [],
      idLed: 0,
      etatLed: false,
      isFavorite: false,
      battery: 100,
      snr: 0,
      rssi: -100,
      infoTemperature: []
    };
  },
  mounted() {
    Mqtt.subscribe(subscribeTopicTemp);
    Mqtt.receive((topicname, message) => {
      if (topicname == subscribeTopicTemp) {
        console.log("la temperature va etre envoyé en bd");
        console.log(topicname);
        console.log(message);

        this.createData(message.valeur, message.type);
      } else if (topicname == "autretest") {
      }
    });
  },
  created() {
    var self = this;

    //this.getTemperatures();

    /*
    setInterval(function() {
      self.getDatas();

      // Temperature
      self.getTemperatures();

      // Lumiere
      self.getLumieres();

      //idLed
      self.getIdLed(); //self.createEtatLed();
    }, 2000);
*/

    // Datas
    self.getDatas();

    // Temperature
    self.getTemperatures();

    // Lumiere
    self.getLumieres();

    //idLed
    self.getIdLed(); //self.createEtatLed();

    // Battery
    setInterval(function() {
      self.battery = Math.round(Math.random() * 100);
    }, 4500);

    // Snr / Rssi
    setInterval(function() {
      self.rssi = (Math.round(Math.random() * 20) + 100) * -1;
      self.snr = Math.round(Math.random() * 20);
    }, 4000);
  }
};
</script>
