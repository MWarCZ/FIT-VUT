<template>
  <!-- App -->
  <f7-app :params="f7params" >

    <!-- Statusbar -->
    <f7-statusbar></f7-statusbar>

    <!-- Left Panel -->
    <f7-panel left reveal theme-dark>
      <f7-view url="/panel-left/"></f7-view>
    </f7-panel>

    <!-- <f7-panel left cover>
      <f7-view url="/panel-left/" links-view=".view-main" />
    </f7-panel> -->

    <!-- Main View -->
    <f7-view id="main-view" url="/" main ></f7-view>

    <!-- <f7-view url="/" :main="true" class="ios-edges"></f7-view> -->

  </f7-app>
</template>
<script>
// Import Routes...
import routes from './routes.js'

export default {
  data() {
    return {
      // Framework7 parameters here
      f7params: {
        id: 'cz.mwarcz.impled', // App bundle ID
        name: 'IMP_LED', // App name
        theme: 'auto', // Automatic theme detection
        // App routes
        routes: routes,
      },
    }
  }
}

</script>
