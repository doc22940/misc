import * as core from '@actions/core';

async function run() {
  try {
    const myInput = core.getInput('status');
    core.debug(`Hello ${myInput}`);
  } catch (error) {
    core.setFailed(error.message);
  }
}

run();
