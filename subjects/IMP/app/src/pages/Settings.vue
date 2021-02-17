<template>
  <f7-page>
    <f7-navbar>
      <f7-nav-left back-link="Back"></f7-nav-left>
      <f7-nav-title>Nastavení</f7-nav-title>
    </f7-navbar>

    <f7-block strong class="text-align-center" style="margin: 0;">
      <f7-block-title class="text-align-center">
        Orientace vykreslovani na displej
      </f7-block-title>

      <br />
      <f7-button big raised fill round color="red"
        @click="uploadRowsOrientatin();">
        Prohodit vykreslování řádků
      </f7-button>
      <br />
      <f7-button big raised fill round color="red"
        @click="uploadColsOrientatin();">
        Prohodit vykreslování sloupců
      </f7-button>
    </f7-block>

  </f7-page>

</template>

<script>
import axios from 'axios'
import { ESP_BASE_URL, axiosConfig } from '@/constants.js'


export default {
  name: 'Settings',
  components: {
  },
  data: function () {
    return {
    }
  },
  methods: {
  	debug (value) {
  		console.log('>', value)
  	},
    uploadRowsOrientatin () {
      this.$f7.dialog.preloader('Odesílá se nastavení...')

      return axios.get(
        ESP_BASE_URL + '/opt/',
        // 'http://httpbin.org/get'+'/text/',
        // 'http://httpbin.org/get',
        {
          timeout: 5 * 1000,
          params: {
            rows: 1,
          },
          crossdomain: true,
          // ...axiosConfig,
        })
        .then(res => {
          this.$f7.dialog.close()
          console.log('OK:', res)
        })
        .catch(e => {
          this.$f7.dialog.close()

          if (e.response) {
            this.$f7.dialog.alert('Server nebyl schopny zpracovat pozadavek.', 'Chyba!')
          } else if (e.request) {
            this.$f7.dialog.alert('Server neodpovídá.', 'Chyba!')
          } else {
            this.$f7.dialog.alert('Nastala nějaká chyba.', 'Chyba!')
          }

          console.log('ERR:', e.request)
        })
    },
    uploadColsOrientatin () {
      this.$f7.dialog.preloader('Odesílá se nastavení...')

      return axios.get(
        ESP_BASE_URL + '/opt/',
        // 'http://httpbin.org/get'+'/text/',
        // 'http://httpbin.org/get',
        {
          timeout: 5 * 1000,
          params: {
            cols: 1,
          },
          crossdomain: true,
          // ...axiosConfig,
        })
        .then(res => {
          this.$f7.dialog.close()
          console.log('OK:', res)
        })
        .catch(e => {
          this.$f7.dialog.close()

          if (e.response) {
            this.$f7.dialog.alert('Server nebyl schopny zpracovat pozadavek.', 'Chyba!')
          } else if (e.request) {
            this.$f7.dialog.alert('Server neodpovídá.', 'Chyba!')
          } else {
            this.$f7.dialog.alert('Nastala nějaká chyba.', 'Chyba!')
          }

          console.log('ERR:', e.request)
        })
    }
  },
}
</script>
