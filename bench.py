#!/usr/bin/env python

import os
import sys
import time
import argparse
import subprocess

def parse_args():
  parser = argparse.ArgumentParser(description='Benchmark a program')
  parser.add_argument('target', metavar='target-program', type=str, help='program which will be benchmarked')
  parser.add_argument('--testcase', '-t', metavar='testcase', type=str, help='testcase directory (default: bin/dp.out)', default='tests/n_10')
  return parser.parse_args()

def run_command(cmd):
  p = subprocess.Popen(' '.join(cmd), shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
  start = time.time()
  stdout_data, stderr_data = p.communicate()
  elapsed_sec = time.time() - start
  return p.returncode, stdout_data, stderr_data, elapsed_sec

def main():
  arg = parse_args()
  if not os.path.exists(arg.target):
    sys.stderr.write("{} does not exist\n".format(arg.target))
    exit(1)
  testcases = list(map(lambda testcase: os.path.join(arg.testcase, testcase), os.listdir(arg.testcase)))
  if len(testcases) == 0:
    sys.stderr.write("no testcase is found\n")
    exit(1)
  sum_sec = 0
  for testcase in testcases:
    sys.stdout.write("running {}...".format(testcase))
    target_status, target_output, target_error, elapsed_sec = run_command([arg.target, testcase])
    if target_status != 0:
      print(" error")
      sys.stderr.write("Reason: target program exited abnormally: exit code {}\n".format(target_status))
      sys.stderr.write("stdout: {}".format(target_output.decode('utf-8')))
      sys.stderr.write("stderr: {}".format(target_error.decode('utf-8')))
      exit(1)
    print(" {:.5f} sec".format(elapsed_sec))
    sum_sec += elapsed_sec
  print("========================================================")
  print("avg: {:.5f} sec".format(sum_sec / len(testcases)))

main()
