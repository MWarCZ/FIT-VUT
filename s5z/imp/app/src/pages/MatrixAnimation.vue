<template>
  <f7-page>
    <f7-navbar>
      <f7-nav-left back-link="Back"></f7-nav-left>
      <f7-nav-title>Vytvořit animaci</f7-nav-title>
      <f7-nav-right>
      </f7-nav-right>
    </f7-navbar>

    <f7-block strong class="text-align-center" style="margin: 0;">

      <f7-swiper scrollbar>
        <f7-swiper-slide v-for="n in screens"
          style="border: 2px solid black; box-sizing: border-box;">
          <f7-list class="mylist" inline-labels no-hairlines-md>
            <f7-list-item>
              <Display
                :matrix="matrixs[n-1]"
                @click="debug(arguments[0]); refreshScreen(arguments[0], matrixs[n-1])"
                />
            </f7-list-item>

            <f7-list-item title="Doba trvání:">
              <f7-stepper big round fill
                :max="1000"
                :min="1"
                :manual-input-mode="true"
                :autorepeat="true"
                :value="durations[n-1]"
                @input="durations[n-1] = Number($event.target.value)"
                >
              </f7-stepper>
            </f7-list-item>

          </f7-list>
        </f7-swiper-slide>
      </f7-swiper>

      <f7-list>
        <f7-list-item title="Počet snímku:">
          <f7-stepper big round fill
            :max="maxScreens"
            :min="0"
            manual-input-mode
            autorepeat
            :value="screens"
            @input="changeScreens($event.target.value)"
            >
          </f7-stepper>
        </f7-list-item>

        <f7-list-item title="Přechod:" smart-select :smart-select-params="{openIn: 'sheet'}">
          <select name="typeAnimation" v-model="typeAnimation">
            <option :value="0">Přeblikávání</option>
            <option :value="1">Posun horizontální</option>
            <option :value="2">Posun vertikálni</option>
          </select>
        </f7-list-item>
      </f7-list>

      <ButtonUpload @upload="uploadScreens()/*uploadScreen()*/"/>
    </f7-block>

  </f7-page>

</template>

<script>
import axios from 'axios'
import { ESP_BASE_URL, axiosConfig } from '@/constants.js'

import Display from '@/components/Display.vue'
import ButtonUpload from '@/components/ButtonUpload.vue'

export default {
  name: 'MatrixAnimation',
  components: {
	 Display,
   ButtonUpload,
  },
  data: function () {
    return {
      screens: 0,
      maxScreens: 50,
      typeAnimation: 0,
      // duration: 10,
      matrixs: [],
      durations: []
    }
  },
  methods: {
  	debug (value) {
  		console.log('>', value)
  	},
    refreshArray (arr) {
      arr.push(1)
      arr.pop()
    },
    row2num(row = []) {
      let num = 0
      row.forEach(c=>{
        num <<= 1
        num |= (c)?1:0
      })
      return num
    },
    refreshScreen (values, matrix) {
      let {row, col, value} = values
      matrix[row][col] =! value
      this.refreshArray(matrix)
    },
    cleanMatrix (matrix) {
      matrix = matrix.map(r=>r.map(()=>false))
    },
    changeScreens (newValue) {
      let screens = Number(newValue)
      let diff = screens - this.screens
      if (diff>0) {
        for(let i = 0; i<diff; i++) {
          this.matrixs.push(Array(8).fill(false).map(x => Array(8).fill(false)))
          this.durations.push(
            (this.durations.length>0)?this.durations[this.durations.length-1]:10
          )
        }
      } else if (diff<0) {
        for(let i = diff; i<0; i++) {
          this.matrixs.pop()
          this.durations.pop()
        }
      }
      this.screens = screens
    },

    uploadScreens: async function () {
      let finished = 0;
      let failedReqArr = []
      let failedLimit = this.screens

      let dialog = this.$f7.dialog.progress('Odesílá se nastavení...', 10)
      dialog.setText('Upload 0/'+(this.screens+1) )
      dialog.setProgress(100/(this.screens+1)*finished)

      for(let n = 0; n<this.screens; n++) {
        console.log(n+') ['+this.durations[n]+']')
        let matrix = this.matrixs[n]

        await axios.get(
          ESP_BASE_URL + '/set/',
          // 'http://httpbin.org/get'+'/text/',
          // 'http://httpbin.org/get',
          {
            timeout: 5 * 1000,
            params: {
              s: n,
              d: this.durations[n],
              r0: this.row2num(matrix[0]),
              r1: this.row2num(matrix[1]),
              r2: this.row2num(matrix[2]),
              r3: this.row2num(matrix[3]),
              r4: this.row2num(matrix[4]),
              r5: this.row2num(matrix[5]),
              r6: this.row2num(matrix[6]),
              r7: this.row2num(matrix[7]),
            },
            crossdomain: true,
            // ...axiosConfig,
          })
          .then(res=>{
            finished++
            dialog.setText('Upload '+finished+'/'+(this.screens+1) )
            dialog.setProgress(100/(this.screens+1)*finished)
          })
          .catch(err => {
            failedLimit--
            failedReqArr.push(err.config)
          })

      }
      await axios.get(
        ESP_BASE_URL + '/set/',
        // 'http://httpbin.org/get'+'/text/',
        // 'http://httpbin.org/get',
        {
          timeout: 5 * 1000,
          params: {
            l: this.screens,
            style: this.typeAnimation,
          },
          crossdomain: true,
          // ...axiosConfig,
        })
        .then(res=>{
          finished++
          dialog.setText('Upload '+finished+'/'+(this.screens+1) )
          dialog.setProgress(100/(this.screens+1)*finished)
        })
        .catch(err => {
          failedLimit--
          failedReqArr.push(err.config)
        })

      failedLimit = 2
      while(failedLimit>0 && failedReqArr.length>0) {
        let req = failedReqArr.pop()
        await axios(req)
          .then(req=>{
            failedLimit = 2
            finished++
            dialog.setText('Upload '+finished+'/'+(this.screens+1) )
            dialog.setProgress(100/(this.screens+1)*finished)
          })
          .catch(err=>{
            failedLimit--
            failedReqArr.push(err.config)
          })
      }
      dialog.close()
      if(finished !== this.screens+1) {
        this.$f7.dialog.alert('Při nahrávání animace došlo k chybě.', 'Chyba!')

      }

    },

  },
}
</script>
