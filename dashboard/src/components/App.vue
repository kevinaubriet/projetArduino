<template>
  <dashboard columns="10" rows="8">
    <value-tile position="a1:b2" heading="Barnsley" color="blue">
      <date-time slot="before" format="ddd DD/MM" time-zone="Europe/London"></date-time>
      <date-time slot="value" format="HH:mm" time-zone="Europe/London"></date-time>
      <weather slot="after" city="Barnsley"></weather>
    </value-tile>

    <indicator-tile position="a3:b3" :value="true" color="green" label="Living room lights"></indicator-tile>
    <indicator-tile position="a4:b4" :value="true" color="blue" label="Garage lights"></indicator-tile>

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

    <sparkline-tile position="c3:d3" :value="temperature" :samples="50"></sparkline-tile>

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
    TextTile
  },
  methods: {
    async update() {
      let result = await dataService.getData();
      console.log(result);
    },
    update2() {
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
        })
        .catch(err => {
          console.log("une erreur est intervenue");
        });
    }
  },

  data() {
    return {
      temperature: 0,
      battery: 100,
      snr: 0,
      rssi: -100,
      chartData1: {},
      chartData2: {},
      chartData3: {},
      info,
      listData: [
        { label: "Something 1", value: 123 },
        { label: "Something 2", value: 90 },
        { label: "Something 3", value: 87 },
        { label: "Something 4", value: 30 },
        { label: "Something 5", value: 10 }
      ]
    };
  },
  mounted() {
    this.update2();
    /*
    fetch("http://localhost:8080/api/temperatures")
      .then(res => res.json())
      .then(res => res.map(console.log(userNames)));
*/
    /*
    var res = dataService.getData();
    console.log(res);

    axios({
      method: "GET",
      url: "http://localhost:8080/api/temperatures"
    }).then(
      result => {
        console.log(result);
        //this.ip = result.data.origin;
      },
      error => {
        console.error(error);
      }
    );
    /*
    axios
      .get("http://localhost:8080/api/temperatures")
      .then(response => (this.info = response));
    print(this.info);*/
  },
  created() {
    var self = this;

    // Temperature
    setInterval(function() {
      self.temperature = Math.random() * 30;
    }, 5000);

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
