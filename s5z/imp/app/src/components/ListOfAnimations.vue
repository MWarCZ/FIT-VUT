<template>
  <f7-page>
    <f7-navbar>
      <f7-nav-left back-link="Back"></f7-nav-left>
      <f7-nav-title>Interni animace</f7-nav-title>
    </f7-navbar>

    <f7-block strong class="text-align-center" style="margin: 0;">

      <f7-row>
        <f7-col>
          <f7-block-title>Nastavení animací</f7-block-title>
          <f7-list class="mylist" inline-labels no-hairlines-md>
            <f7-list-item title="Přechod:" smart-select :smart-select-params="{openIn: 'sheet'}">
              <select name="typeAnimation" v-model="typeAnimation">
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
            <f7-list-item title="Animace:" smart-select :smart-select-params="{openIn: 'sheet'}">
              <select name="animation" v-model="animation">
                <option v-for="i in animations"
                  :value="i.id">{{i.name}}</option>
              </select>
            </f7-list-item>
          </f7-list>
          <ButtonUpload @upload="emitSelect(animation)"/>
        </f7-col>
      </f7-row>
    </f7-block>

  </f7-page>
</template>

<script>
import ButtonUpload from '@/components/ButtonUpload.vue'

export default {
  name: 'ListOfAnimations',
  components: {
    ButtonUpload
  },
  props: {
    maxAnimations: {
      default: 0
    },
    animations: {
      default: ()=>[]
    },
  },
  data: function () {
    return {
      typeAnimation: 0,
      duration: 10,
		  animation: 3,
    };
  },
  methods: {
    emitSelect (selectedAnimatin) {
      this.$emit('select', {
        selected: selectedAnimatin,
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
