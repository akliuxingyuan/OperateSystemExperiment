<template>
  <div class="container">
    <vue-gallery :photos="photos"></vue-gallery>
    <!--绑定属性photos，这里简写-->
  </div>
</template>

<script>
import VueGallery from "@/components/VueGallery.vue";

export default {
  name: "Photo",
  components: {
    VueGallery,
  },
  data: function () {
    //
    return {
      photos: [
        require("../assets/img/lordea-home-01-min.jpg"), //vue中background-image图片路径问题,动态路径，可以使用require()返回图片路径。
        require("../assets/img/lordea-home-02-min.jpg"),
        require("../assets/img/lordea-home-03-min.jpg"),
        require("../assets/img/lordea-home-04-min.jpg"),
        require("../assets/img/lordea-home-05-min.jpg"),
        require("../assets/img/lordea-home-06-min.jpg"),
        require("../assets/img/lordea-home-07-min.jpg"),
        require("../assets/img/lordea-home-08-min.jpg"),
      ],
    };
  },
};
</script>

