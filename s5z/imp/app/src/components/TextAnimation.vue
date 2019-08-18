<template>
  <f7-page>
    <f7-navbar>
      <f7-nav-left back-link="Back"></f7-nav-left>
      <f7-nav-title>Text</f7-nav-title>
      <f7-nav-right>
        <ButtonUpload @upload="emitUpload()"/>
      </f7-nav-right>
    </f7-navbar>

    <f7-block strong class="text-align-center" style="margin: 0;">

      <f7-row>
        <f7-col>
          <f7-block-title>Vlastní text na displeji</f7-block-title>
          <f7-list class="mylist" inline-labels no-hairlines-md>
            <f7-list-item title="Text:">
              <f7-list-input
                label=""
                type="text"
                placeholder="Toto se zobrazi na displeji."
                clear-button
                :maxlength="60"
                :value="text"
                @input="text = $event.target.value"
              >
              </f7-list-input>
            </f7-list-item>
            <f7-list-item title="Přechod:" smart-select :smart-select-params="{openIn: 'sheet'}">
              <select name="mac-windows" v-model="typeAnimation">
                <option :value="0">Přeblikávání</option>
                <option :value="1">Posun horizontální</option>
                <option :value="2">Posun vertikálni</option>
              </select>
            </f7-list-item>
            <f7-list-item title="Doba trvání:">
              <f7-stepper big round fill
                :max="1000"
                :min="1"
                :manual-input-mode="true"
                :autorepeat="true"
                :value="duration"
                @input="duration = Number($event.target.value)"
                >
              </f7-stepper>
            </f7-list-item>
            <f7-list-item v-if="false" title="XXX:">
              {{'"'+text+'" '+typeAnimation+' '+duration}}
            </f7-list-item>
          </f7-list>
        </f7-col>
      </f7-row>
      <f7-row>
        <f7-col>
          <ButtonUpload @upload="emitUpload()"/>
        </f7-col>
      </f7-row>
    </f7-block>

  </f7-page>
</template>

<script>
import { f7ListInput } from 'framework7-vue';
import ButtonUpload from '@/components/ButtonUpload.vue'

export default {
  name: 'TextAnimation',
  components: {
    ButtonUpload
  },
  props: {
  },
  data: function () {
    return {
      text: "ABCDE 12345.",
      typeAnimation: 1,
      duration: 10,
    };
  },
  methods: {
    emitUpload () {
      this.$emit('upload', {
        text: this.text,
        typeAnimation: this.typeAnimation,
        duration: this.duration,
      })
    }
  },
}
</script>
<style scoped>
.mylist > ul > li {
  box-shadow: 0px -1px black;
}
.mylist > ul > li:last-child {
  box-shadow: 0px -1px black, 0px 1px black;
}

</style>
