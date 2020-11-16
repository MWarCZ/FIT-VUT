<template>
  <f7-page>
    <f7-navbar>
      <f7-nav-left>
        <f7-link icon-if-ios="f7:menu" icon-if-md="material:menu" panel-open="left"></f7-link>
      </f7-nav-left>
      <f7-nav-title>IMP Home</f7-nav-title>
      <f7-nav-right>
        <f7-link icon-if-ios="f7:settings" icon-if-md="material:settings" href="/settings/"></f7-link>
      </f7-nav-right>
    </f7-navbar>


    <f7-block strong style="margin: 0;">
      <f7-block-title class="text-align-center">
        Výběr akcí
      </f7-block-title>

      <f7-row >
        <f7-col>
          <f7-button big raised outline color="black"
          href="/live/">
            LED Matice
          </f7-button>
        </f7-col>
        <f7-col>
          <f7-button big raised outline color="black"
          href="/text/">
            Nahrát text
          </f7-button>
        </f7-col>
      </f7-row>
      <br />
      <f7-row >
        <f7-col>
          <f7-button big raised outline color="black"
          href="/list/">
            Vybrat animaci
          </f7-button>
        </f7-col>
      </f7-row>
    </f7-block>

  </f7-page>
</template>
<script>

export default {
  name: 'HomePage',
  props: {
  },
  data: function () {
    return {
    };
  },
  methods: {
  },
}
</script>
