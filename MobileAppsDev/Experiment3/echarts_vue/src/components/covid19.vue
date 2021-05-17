<template>
  <div class="hello">
    <mytop />
    <div
      id="mychart"
      ref="mapbox"
      style="
        margin-top: 10px;
        width: 100%;
        min-width: 200px;
        height: 100%;
        min-height: 200px;
      "
    ></div>
  </div>
</template>

<script>
import mytop from "./top.vue";
import echarts from "echarts";
import "echarts/map/js/china";
import jsonp from "jsonp";

const option = {
  title: {
    text: "疫情地图",
    link: "http://tangdd.cn",
    subtext: "全国疫情地图一览表",
  },
  series: [
    {
      data: [],
      type: "map",
      map: "china",
      label: { show: true, color: "black", fontSize: 10 },
      itemStyle: {
        areaColor: "pink",
        borderColor: "#776a6a",
      },
      emphasis: {
        label: {
          color: "black",
          fontSize: 12,
        },
        itemStyle: {
          areaColor: "#ccc",
        },
      },
      zoom: 1,
    },
  ],

  visualMap: [
    {
      zoom: 1,
      type: "piecewise",
      show: true,
      splitNumber: 5,
      pieces: [
        { min: 10000 },
        { min: 1000, max: 9999 },
        { min: 100, max: 999 },
        { min: 10, max: 99 },
        { min: 1, max: 9 },
      ],
      align: "right",
      inRange: {
        symbol: "rect",
        color: ["#ffc9c9", "#9c0505"],
      },
      itemWidth: 8,
      itemHeight: 4,
    },
  ],
};
export default {
  name: "HelloWorld",
  components: {
    mytop,
  },
  mounted() {
    this.getData();
    this.mychart = echarts.init(this.$refs.mapbox);
    this.mychart.setOption(option);
    this.resizeTheChart();
    window.addEventListener("resize", this.resizeTheChart);
  },
  methods: {
    resizeTheChart() {
      if (this.$refs && this.$refs.mapbox) {
        let mychartNode = document.getElementById("mychart");
        mychartNode.style.height = mychartNode.offsetWidth * 0.8 + "px";
        this.mychart.resize();
      }
    },
    // 接口采用自'https://renjinhui.github.io/review_vue/dist/index.html#/yqdt'
    getData() {
      jsonp(
        "https://interface.sina.cn/news/wap/fymap2020_data.d.json?_=1580892522427&callback=__jp0",
        {},
        (err, data) => {
          if (!err) {
            console.log(data);
            let list = data.data.list.map((item) => ({
              name: item.name,
              value: item.value,
            }));
            option.series[0].data = list;
            console.log(list);
            this.mychart.setOption(option);
          }
        }
      );
    },
  },
};
</script>

<!-- Add "scoped" attribute to limit CSS to this component only -->
<style scoped lang="less">
* {
  padding: 0;
  margin: 0;
}
h3 {
  margin: 40px 0 0;
}
ul {
  list-style-type: none;
  padding: 0;
}
li {
  display: inline-block;
  margin: 0 10px;
}
a {
  color: #42b983;
}
</style>
