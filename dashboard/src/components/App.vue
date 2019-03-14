<template>
  <dashboard columns="10" rows="8">
    <value-tile position="a1:b2" heading="Barnsley" color="blue">
      <date-time slot="before" format="ddd DD/MM" time-zone="Europe/London"></date-time>
      <date-time slot="value" format="HH:mm" time-zone="Europe/London"></date-time>
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
      <percentile-change slot="after" :value="temperature"></percentile-change>
    </value-tile>

    <gauge-tile
      position="c4:d5"
      color="green"
      :value="temperature"
      :max="30"
      :decimal-places="2"
      unit="°C"
    ></gauge-tile>

    <value-tile
      position="e1:f2"
      heading="Lumiere"
      color="yellow"
      :value="temperature"
      :decimal-places="2"
      unit="°C"
    >
      <percentile-change slot="after" :value="temperature"></percentile-change>
    </value-tile>
    <level-tile
      position="e3:e5"
      color="yellow"
      label="Temperature"
      :max="30"
      :value="temperature"
      unit="°C"
    ></level-tile>
    <level-tile
      position="f3:f5"
      color="green"
      label="Battery"
      :min="0"
      :max="100"
      :value="battery"
      unit="%"
    ></level-tile>

    <list-tile position="g1:h3" heading="Something" color="red" :values="listData"></list-tile>
    <text-tile position="g4:h5" value="Lorem ipsum dolar sit amet consectetur adipiscing elit"></text-tile>
    <!--
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
    <chart-tile position="a6:f8" heading="Something" color="red" :data="chartData1" type="bar"></chart-tile>
    <chart-tile position="g6:j8" heading="Something" color="blue" :data="chartData1" type="line"></chart-tile>
  </dashboard>
</template>

<script>
//"infoTemperature[0].valeur"
import Dashboard from "./Dashboard";

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

const dataService = new DataSerice();

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
    getTemperatures() {
      let url = "http://localhost:8081/api/temperatures";
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
          console.log(data[0].valeur);
          this.temperature = parseFloat(data[0].valeur);
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
    getEtatLed() {
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
        .then(responseJS => {
          console.log(responseJS.msg);
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
          console.log(this.idLed);
          this.getEtatLed();
        })
        .catch(err => {
          console.log("une erreur est intervenue");
        });
    },
    modifEtatButton() {
      this.etatLed = !this.etatLed;
      this.modifEtatLed();
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
        .then(responseJS => {
          console.log(responseJS.msg);
        })
        .catch(err => {
          console.log(err);
        });
    }
  },

  data() {
    return {
      temperature: 0,
      idLed: 0,
      etatLed: false,
      isFavorite: false,
      battery: 100,
      snr: 0,
      rssi: -100,
      chartData1: {},
      chartData2: {},
      chartData3: {},
      infoTemperature: [],
      listData: [
        { label: "Something 1", value: 123 },
        { label: "Something 2", value: 90 },
        { label: "Something 3", value: 87 },
        { label: "Something 4", value: 30 },
        { label: "Something 5", value: 10 }
      ]
    };
  },
  mounted() {},
  created() {
    var self = this;

    // Temperature
    self.getTemperatures();

    //idLed
    self.createEtatLed();

    // Battery
    setInterval(function() {
      self.battery = Math.round(Math.random() * 100);
    }, 4500);

    // Snr / Rssi
    setInterval(function() {
      self.rssi = (Math.round(Math.random() * 20) + 100) * -1;
      self.snr = Math.round(Math.random() * 20);
    }, 4000);

    self.chartData1 = {
      labels: [
        "Jan",
        "Feb",
        "Mar",
        "Apr",
        "May",
        "Jun",
        "Jul",
        "Aug",
        "Sep",
        "Oct",
        "Nov",
        "Dec"
      ],
      datasets: [
        {
          label: "Test",
          color: "#2ecc71",
          data: [10, 15, 20, 20, 10, 15, 5, 7, 30, 10, 12, 15]
        },
        {
          label: "Test2",
          color: "#3498db",
          data: [10, 15, 5, 7, 30, 10, 12, 15, 10, 15, 20, 20]
        }
      ]
    };

    self.chartData3 = {
      labels: ["Jan", "Feb", "Mar"],
      datasets: [
        {
          label: "Test",
          color: ["#e74c3c", "#3498db", "#2ecc71"],
          data: [10, 15, 20]
        }
      ]
    };
  }
};
</script>
