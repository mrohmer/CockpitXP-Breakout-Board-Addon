<script lang="ts">
  import "../app.pcss";
</script>

<div class="container mx-auto px-4 py-8 space-y-14">
    <div>
        <h1 class="headline">
            <span class="block">
                <span class="letter">C</span><br>
                <span class="complete">Carrera</span>
            </span>
            <span class="block">
                <span class="letter">M</span><br>
                <span class="complete">Männer</span>
            </span>
            <span class="block">
                <span class="letter">A</span><br>
                <span class="complete">Abend</span>
            </span>
        </h1>
    </div>
    <div>
        <slot/>
    </div>
</div>

<style lang="postcss">
    .headline {
        @apply text-center text-transparent bg-clip-text w-fit mx-auto flex;
        background-image: linear-gradient(
                to right,
                #462523 0,
                #cb9b51 22%,
                #f6e27a 45%,
                #f6f2c0 50%,
                #f6e27a 55%,
                #cb9b51 78%,
                #462523 100%
        );
        -webkit-text-fill-color: transparent;
    }

    .letter {
        @apply text-[100px] leading-[85px] font-bold;
    }
    .complete {
        @apply text-[25px] leading-[30px] font-light;
    }
</style>
